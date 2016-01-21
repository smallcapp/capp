#ifndef __THIS_APP
#define __THIS_APP      umd
#endif

#ifndef __THIS_FILE
#define __THIS_FILE     "flow"
#endif

#include "umd.h"

static struct um_flow flow;

static inline bool
is_local_mac(unsigned char mac[])
{
    return os_maceq(mac, umd.intf[UM_INTF_TC].mac);
}

static inline bool
is_local_ip(unsigned int ip)
{
    return ip==umd.intf[UM_INTF_TC].ip;
}

static inline bool
is_user_ip(unsigned int ip)
{
    struct um_intf *intf = &umd.intf[UM_INTF_TC];
    
    return (ip & intf->mask)==(intf->ip & intf->mask);
}

static int
pkt_recv(simpile_server_t *server)
{
    int err, len;
    socklen_t addrlen = sizeof(server->addr.ll);

    err = recvfrom(server->fd, flow.packet, sizeof(flow.packet), 0, &server->addr.c, &addrlen);
    if (err<0) {
        return err;
    }
    flow.len = err;

    if (__is_debug_level_packet) {
        os_println("recv packet length=%d", flow.len);
        __os_dump_buffer(flow.packet, flow.len, NULL);
    }
    
    return 0;
}

static int
intf_check(simpile_server_t *server)
{
    if (server->addr.ll.sll_ifindex == umd.intf[UM_INTF_TC].index) {
        return 0;
    } else {
        return -EBADIDX;
    }
}

static int
eth_check(simpile_server_t *server)
{
    struct ether_header *eth = flow.eth = (struct ether_header *)flow.packet;
    unsigned char *smac = eth->ether_shost;
    unsigned char *dmac = eth->ether_dhost;
    
    if (eth->ether_type != flow.ether_type_ip &&
        eth->ether_type != flow.ether_type_vlan) {
        return -EFORMAT;
    }
#if 1
    else if (false==is_good_mac(smac)) {
        return -EFORMAT;
    }
    else if (false==is_good_mac(dmac)) {
        return -EFORMAT;
    }
#endif
    else if (is_local_mac(smac)) {
        flow.dir = um_flow_dir_down;
        flow.usermac = dmac;
    }
    else if (is_local_mac(dmac)) {
        flow.dir = um_flow_dir_up;
        flow.usermac = smac;
    }
    else {
        return -EFORMAT;
    }

    flow.eth_packets++;
    flow.eth_bytes += flow.len;

    if (__is_debug_level_packet) {
        char smacstring[1+MACSTRINGLEN_L];
        char dmacstring[1+MACSTRINGLEN_L];
        char  macstring[1+MACSTRINGLEN_L];
        
        os_strcpy(smacstring, os_macstring(eth->ether_shost));
        os_strcpy(dmacstring, os_macstring(eth->ether_dhost));
        os_strcpy( macstring, os_macstring(flow.usermac));
        
        debug_packet("recv packet smac=%s dmac=%s type=0x%x dir=%s usermac=%s", 
            smacstring,
            dmacstring,
            ntohs(eth->ether_type),
            flow_dir_string(flow.dir),
            macstring);
    }
    
    return 0;
}

static int
vlan_check(simpile_server_t *server)
{
    if (flow.eth->ether_type == flow.ether_type_vlan) {
        struct vlan_header *vlan = flow.vlan = (struct vlan_header *)(flow.eth+1);

        if (vlan->type != flow.ether_type_ip) {
            return -EFORMAT;
        }

        flow.vlan_packets++;
        flow.vlan_bytes += flow.len;

        debug_packet("recv packet vlan=%d type=%d", 
            vlan->vid,
            vlan->type);
    }
    
    return 0;
}

static int
ip_check(simpile_server_t *server)
{
    struct ip *iph;

    if (flow.eth->ether_type == flow.ether_type_ip) {
        iph = flow.iph = (struct ip *)(flow.eth+1);
    }
    else if (flow.eth->ether_type == flow.ether_type_vlan) {
        iph = flow.iph = (struct ip *)(flow.vlan+1);
    }
    else {
        return -EFORMAT;
    }

    unsigned int sip = iph->ip_src.s_addr;
    unsigned int dip = iph->ip_dst.s_addr;
    
    if (4!=iph->ip_v) {
        return -EFORMAT;
    }
    else if (5!=iph->ip_hl) {
        return -EFORMAT;
    }
    else if (is_local_ip(sip)) {
        if (is_local_ip(dip)) {
            return -EFORMAT;
        }
        else if (is_user_ip(dip)) {
            flow.type = um_flow_type_lan;
            flow.userip = dip;
        }
        else {
            flow.type = um_flow_type_local;
            
            return -EFORMAT;
        }
    }
    else if (is_user_ip(sip)) {
        if (is_local_ip(dip)) {
            flow.type = um_flow_type_lan;
            flow.userip = sip;
        }
        else if (is_user_ip(dip)) {
            return -EFORMAT;
        }
        else {
            flow.type = um_flow_type_wan;
            flow.userip = sip;
        }
    }
    else {
        if (is_local_ip(dip)) {
            flow.type = um_flow_type_local;
            
            return -EFORMAT;
        }
        else if (is_user_ip(dip)) {
            flow.type = um_flow_type_wan;
            flow.userip = dip;
        }
        else {
            return -EFORMAT;
        }
    }

    flow.ip_packets++;
    flow.ip_bytes += flow.len;
    
    if (__is_debug_level_packet) {
        char sipstring[1+OS_IPSTRINGLEN];
        char dipstring[1+OS_IPSTRINGLEN];
        char  ipstring[1+OS_IPSTRINGLEN];
        
        os_strcpy(sipstring, os_ipstring(iph->ip_src.s_addr));
        os_strcpy(dipstring, os_ipstring(iph->ip_dst.s_addr));
        os_strcpy( ipstring, os_ipstring(flow.userip));
        
        debug_packet("recv packet sip=%s dip=%s protocol=%d type=%s userip=%s",
            sipstring,
            dipstring,
            iph->ip_p,
            flow_type_string(flow.type),
            ipstring);
    }

    return 0;
}

static inline bool
__is_flow_reauth(struct um_user *user, int type, int dir)
{
    uint64_t reauth = __flow_reauth(user, type, dir);
    uint64_t now    = __flow_now(user, type, dir);
    
    bool is = reauth && reauth < now;
    if (is) {
        debug_flow("user(%s) type(%s) dir(%s) flow reauth(%llu) > now(%llu)",
            os_macstring(user->mac),
            flow_type_string(type),
            flow_dir_string(dir),
            reauth,
            now);
    }
    
    return is;
}

static inline bool
is_flow_reauth(struct um_user *user, int type, int dir)
{
    return __is_flow_reauth(user, type, dir)
        || __is_flow_reauth(user, type, um_flow_dir_all);
}

static void
flow_reauth(struct um_user *user, int type, int dir)
{
    if (um_flow_type_wan==type && is_auth(user) && is_flow_reauth(user, type, dir)) {
        user_reauth(user);
    }
}

static inline bool
__is_overflow(struct um_user *user, int type, int dir)
{
    uint64_t max    = __flow_max(user, type, dir);
    uint64_t now    = __flow_now(user, type, dir);
    
    bool is = max && max < now;
    if (is) {
        debug_flow("user(%s) type(%s) dir(%s) flow max(%llu) > now(%llu)",
            os_macstring(user->mac),
            flow_type_string(type),
            flow_dir_string(dir),
            max,
            now);
    }

    return is;
}

static inline bool
is_overflow(struct um_user *user, int type, int dir)
{
    return __is_overflow(user, type, dir)
        || __is_overflow(user, type, um_flow_dir_all);
}

static void
overflow(struct um_user *user, int type, int dir)
{
    if (um_flow_type_wan==type && is_auth(user) && is_overflow(user, type, dir)) {
        user_deauth(user, UM_DEAUTH_FLOWLIMIT);
    }

    if (is_overflow(user, um_flow_type_lan, dir)) {
        user_unbind(user, UM_DEAUTH_FLOWLIMIT);
    }
}

static void
flow_update(struct um_user *user, int type, int dir)
{
    if (um_flow_dir_all!=dir) {
        __flow_now(user, type, dir) += flow.len;
    }
    __flow_now(user, type, um_flow_dir_all) += flow.len;

    if (um_flow_type_wan==type) {
        flow_update(user, um_flow_type_lan, dir);
    }
}

static int
flow_check(simpile_server_t *server)
{
    struct um_user *user;

#if UM_USE_AUTO
    user = um_user_bind(flow.usermac, flow.userip);
#else
    user = um_user_get(flow.usermac);
#endif
    if (NULL==user) {
        return -ENOEXIST;
    }

    if (have_bind(user)) {
        flow_update(user, flow.type, flow.dir);
        um_user_debug(user, "user-flow-update", __is_debug_level_flow);

        flow_reauth(user, flow.type, flow.dir);
        overflow(user, flow.type, flow.dir);

        update_aging(user, false);
    }

    return 0;
}

jobj_t
um_jflow(void)
{
    jobj_t obj = jobj_new_object();
    jobj_t eth = jobj_new_object();
    jobj_t vlan= jobj_new_object();
    jobj_t ip  = jobj_new_object();

    jobj_add_int(eth, "packets", flow.eth_packets);
    jobj_add_int64(eth, "bytes", flow.eth_bytes);
    jobj_add(obj, "eth", eth);

    jobj_add_int(vlan, "packets", flow.vlan_packets);
    jobj_add_int64(vlan, "bytes", flow.vlan_bytes);
    jobj_add(obj, "vlan", vlan);

    jobj_add_int(ip, "packets", flow.ip_packets);
    jobj_add_int64(ip, "bytes", flow.ip_bytes);
    jobj_add(obj, "ip", ip);

    return obj;
}

static int
flow_server_init(simpile_server_t *server)
{
    int fd, err;

    flow.ether_type_ip  = htons(ETHERTYPE_IP);
    flow.ether_type_vlan= htons(ETHERTYPE_VLAN);
    flow.ether_type_all = htons(ETH_P_ALL);
    
    fd = socket(AF_PACKET, SOCK_RAW, flow.ether_type_all);
    if (fd<0) {
        debug_error("socket error:%d", -errno);

        return -errno;
    }
    
    fcntl(fd, F_SETFL, O_NONBLOCK);
    
#if UM_USE_BINDIF
    struct sockaddr_ll addr = {
        .sll_family     = AF_PACKET,
        .sll_protocol   = flow.ether_type_all,
        .sll_ifindex    = umd.intf[UM_INTF_TC].index,
    };
    err = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (err) {
        return err;
    }
#endif

    server->fd = fd;
    
    debug_ok("init flow server ok");
    
    return 0;
}

static int
__flow_server_handle(simpile_server_t *server)
{
    static int (*handle[])(simpile_server_t *) = {
        pkt_recv,
        intf_check,
        eth_check,
        vlan_check,
        ip_check,
        flow_check,
    };
    int i, err;

    for (i=0; i<os_count_of(handle); i++) {
        err = (*handle[i])(server);
        if (err) {
            return err;
        }
    }
    
    return 0;
}

static int
flow_server_handle(simpile_server_t *server)
{
    int i, err;

    for (i=0; i<UM_SNIFF_COUNT; i++) {
        err = __flow_server_handle(server);
        if (err) {
            return err;
        }
    }

    return 0;
}

simpile_server_t um_flow_server = {
    .fd     = INVALID_FD,
    .addr   = OS_SOCKADDR_INITER(AF_PACKET),
    
    .init   = flow_server_init,
    .handle = flow_server_handle,
};

/******************************************************************************/
