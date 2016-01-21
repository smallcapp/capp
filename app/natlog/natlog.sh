#!/bin/bash

main(){

	local newfile="/tmp/natloag_new.txt"
	local oldfile="/tmp/natloag_old.txt"
	local lastfile="/tmp/natloag_release.txt"

	if [ ! -f ${newfile} ] && [ ! -f ${oldfile}]; then
		cat /proc/net/ip_conntrack > ${lastfile};	
	else
		mv ${newfile} ${oldfile}		
		cat /proc/net/ip_conntrack > ${newfile};	
		/usr/sbin/natlog ${oldfile} ${newfile} ${lastfile}  
	fi
}
main "$@"
