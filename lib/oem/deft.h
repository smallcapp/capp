#ifndef __LIB_OEM_DEFT_H__
#define __LIB_OEM_DEFT_H__
/******************************************************************************/
#define OEM_NAME    "default"
#define OEM_MAC     ""
#define OEM_SN      ""
#define OEM_TYPE    ""
#define OEM_VERSION ""

#define OEM_BASE_INITER __OEM_BASE_INITER( \
    OEM_NAME,   \
    OEM_MAC,    \
    OEM_SN,     \
    OEM_TYPE,   \
    OEM_VERSION)

#define OEM_LSS_USER            "LSS"
#define OEM_LSS_PWD             "LTEFISecurityServer2012-2015"
#define OEM_LSS_SERVER          "lms2.autelan.com"
#define OEM_LSS_PORT            "9999"

#define OEM_LSS_INITER __OEM_LSS_INITER( \
    OEM_LSS_USER,   \
    OEM_LSS_PWD,    \
    OEM_LSS_SERVER, \
    OEM_LSS_PORT)

#define OEM_RSYNC_USER          "rsync"
#define OEM_RSYNC_PWDFILE       "bHRlZmlWMnJzeW5jUFdECg=="
#define OEM_RSYNC_PATH          "version/lte-fi2/mdboard"
#define OEM_RSYNC_SERVER        "lms3.autelan.com"
#define OEM_RSYNC_PORT          "873"
#define OEM_RSYNC_TIMEOUT       "300"

#define OEM_RSYNC_INITER  __OEM_RSYNC_INITER( \
    OEM_RSYNC_USER,     \
    OEM_RSYNC_PWDFILE,  \
    OEM_RSYNC_PATH,     \
    OEM_RSYNC_SERVER,   \
    OEM_RSYNC_PORT,     \
    OEM_RSYNC_TIMEOUT)

// deft/lms/ca.crt.base64
#define OEM_LMS_CA \
    "LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSURMRENDQWhTZ0F3SUJBZ0lKQU5QdEx2RXEw" \
    "Mm9nTUEwR0NTcUdTSWIzRFFFQkJRVUFNQmd4RmpBVUJnTlYKQkFNVURTb3VZWFYwWld4aGJpNWpi" \
    "MjB3SGhjTk1UVXhNakEwTVRreU5EUTBXaGNOTWprd09ERXlNVGt5TkRRMApXakFZTVJZd0ZBWURW" \
    "UVFERkEwcUxtRjFkR1ZzWVc0dVkyOXRNSUlCSWpBTkJna3Foa2lHOXcwQkFRRUZBQU9DCkFROEFN" \
    "SUlCQ2dLQ0FRRUF5UGs1c2V1Umh1OTgyVEk4ZjVKdEgxaEU1eFMvQUNCUFk5clZNRVJLNnhCZVhu" \
    "Q2QKcFRPczhiR3hpam1ZUWl1TkExWnloeGFiaWpkdThJWnhUektoSU9zRkpKWFg0YVk0YlMrTUQw" \
    "cVVMNHM0Y1FqZgpKVWRSbXoyQ1dOUzBDam5SMy9XdFliZVpicGhJQUNKWnFMc3dJUWhBK3p1aDdt" \
    "YUN1WnhXWStST21aUE8yL1ZtCnlHSy9RT05nVmJSRDZYSHZ3bnZhTFA3RmMwdUF0OW5GUklIY1lS" \
    "bWU3NnhQZjduTWF3enVybzVZTHB3b1dnMkEKUUZ6ZTN6NlZRUmNndG5YTTRlNzFDL25VWVVaaVI3" \
    "Wm1SYTlKV1kvVmhKTGxzVDlueGhnWWJjN0tvQmdKRjJIUApKaW9HVTMxa2I2bTRZbG5jc3MraDBs" \
    "NWlaTGIyZ1RndEx2dDdld0lEQVFBQm8za3dkekFkQmdOVkhRNEVGZ1FVCitVNVdZNUZJMSt4MGpT" \
    "MlZHb3NrQ2dEMUZTOHdTQVlEVlIwakJFRXdQNEFVK1U1V1k1RkkxK3gwalMyVkdvc2sKQ2dEMUZT" \
    "K2hIS1FhTUJneEZqQVVCZ05WQkFNVURTb3VZWFYwWld4aGJpNWpiMjJDQ1FEVDdTN3hLdE5xSURB" \
    "TQpCZ05WSFJNRUJUQURBUUgvTUEwR0NTcUdTSWIzRFFFQkJRVUFBNElCQVFDSHEyeHhxb0IrODdw" \
    "WEdqTm9jaXVICmk0blFLVDVHazZXY092eEZGK29GWlR3TjRYRGprbmFvbEtGMmgyWnlVY2dDK1Rv" \
    "SklpTnM2TkNGa2xYQ2xjaUgKNnh0QUpFcE5hdDRIckttMFBQNzBBbXpsendZRTNSY2RYSE90ekU3" \
    "Sk9aUTBOOFNNaTNvcE1ibUJtaXdKWGFldgphNWthSGVnZTJFMm9KcXhuV2hJem0vOEZONDJiZFg3" \
    "ZmhEOGIxU2l3NkFZMTd0VHI2MWpmS3ozVzJWUnl6VlBVCkNzUGNsbGE4UVdlSndQVzErZEt1K3hx" \
    "YUhzeTJvMS9tazF2Q1FPLzVwKzdpQmJlTFBNNlZRL0toTE5NVlJwZmcKbW1CRlJwcENFbzlucWl3" \
    "N1FXVTZ5eHNyaXhJb2x6UThoMDRtTkNhcVV0d0FRRFJnSkFKa1BXRW9QdE94QzZrZwotLS0tLUVO" \
    "RCBDRVJUSUZJQ0FURS0tLS0tCg==" \
    /* end */
    
// deft/lms/client.key.base64
#define OEM_LMS_KEY \
    "LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFcEFJQkFBS0NBUUVBd1BOTkpjbTFO" \
    "VjdYa3NTNGN2Wjd4bkZsd05tSVNyUnlZalJsUEZ0THZCY2h0Q3lrCkFScktna25GRFdTcTZibHVn" \
    "WUlKdkZjZTYzMkJKdVBZbkhpdFRmd0UzeVdYS1V0SndGNlovT2R5ZU9uUjdxVTUKTXNuMFNScXBY" \
    "V1ZCMExBc2o0WUFkU2Nrc21zWHozODhkN1UxV3QzdXFjVE5zbWxmRUJQdVRaWVFTaE1ZMVBHcwpn" \
    "Z3l0ZnJMTnorbkFqa09pRDZVbi9Xd0V1WjVDR2c5RDNkUFUza1JMOXhGQkx6ZG1USlNEVFN2TmVM" \
    "bkRUeXVUClhzSmtKYmdWZWttUDIxaUhSQUxQTFoyZGxucWdSc2ZGUmEzSnJNYXYzQ0laSHpPY2Z3" \
    "UGZuMk1qL0RxMXlkQ1oKakFlUjJiTXZmczA0bTFzbGFMOXhYT3RaZTNaNVJoMC9XTDdsbndJREFR" \
    "QUJBb0lCQVFDSHdHWU42UTFhQUdGUgpQS0M5R1pmWjlWeHV3MTIxbWFGRVlMSzlOVXBpaWkvSFVR" \
    "VU9WZWdXcElwRU13aGdxbnNLR1JCNmR4MGs5OGNQCk1venJBYWlFem9RcTc0cWJuZ0hFbTRwaHVn" \
    "RDYreDhkZFJkNkdVYUt6SHhPWG5oZ3VDS1IwK2lzaEI2NGZNcGkKelpVZ2I5aWJGcnZjcTc5QkFB" \
    "dkhCdzdKZllQMmFKK0QzeHdoYXo5V2FLWHUwTG16K201ZUdJcS9NUlMvdWlocQp1SmR2bncrSE1H" \
    "N3Z1TzdpeDAxTjh2SC9zOXV3QkR6U0JXNHhOVjVSZTVHVFEwZHppUGlVNjhteFZ3NWF5UEttCkdT" \
    "bkF0N1YvaVQxdld1c2QwUEovZUM4S21kb0dTRDRjSnBFZnJwc3ZOQzJIb0JhMVBrbnIweDVZQ1Iv" \
    "VytEa1QKQ1I2VTNUcVpBb0dCQVA4Rmt4WjEraHoxU3ZUM2xPOG94ZHRrK2VSSVZrVGM0K3hycmFU" \
    "MkNybFpyTUN5MFJreQp0ZTE3QmNBbDRibmhnek9oSHdrU0wyQWR5RHljZEJpOGJseENwcmU4Y3RM" \
    "cFpwWE1QL29FbXlIR1lrOXZyY2N1Ck1TNmZCQUtrNUhOeEtOeHFaTUJIZngrUlM2eThBeVMxOWJP" \
    "YW1OalBEYy9QVytTUGFzQ3FsUVR0QW9HQkFNR3cKeGk2WkpJc2tYUi9HampaWWZrSFVwM2txQWJ3" \
    "UFpCVW1kaFd5TmdLbUhDZUFNUDdscTBpNFBTY2hvMTQvL0RBUwphdkpBbTBIME4zT2wyUURwc0xo" \
    "Y2hGaWQ4NVFCZlhwTUFWWEFRekpDODdRS09iQU01RDhYVEwxZ21uclNITWRvCjNmeUljKzZxcmVy" \
    "L0UyVjU2ZmMrTGhjYlkrL3QrL2R6T05MVDFHODdBb0dBQmRGRHZMUnVRekRaUitOSjgzYXoKcE1D" \
    "VjdyNHpzWmgwWHBWS1BjUWJGSVZXS29NcHYvQm5OQWFsWisrMVNYODZkZmdMOURJRkFLVk5Sd3l1" \
    "blUzWApHcVd1bGV4bmxpWWEzL25yejdGQk5MRUkxeFVPR3NnVCtTTkpmU0h6MENwWnEwV0dETmpV" \
    "cm95cTlFTEpzNUd2CkRDelBwY3A3WHkwMHhHNzlHVzRLdmpVQ2dZQS9BUWVXUk01MFRHSFdZam9F" \
    "UjRqYVZSZ2MrNk1RejFqWTRHZnMKNnNsUzEwRzFQNmV3WW5qeThLdER2VmRzNHhkZmM3NUNDdTMx" \
    "NkVEWlo0dE5GcmNiWVV6VG5wMksrV2tkalBlcQpNeDRvalVyRXQ4YklFNGx6VkZyc0J5NTQ0VHZu" \
    "U1o5YjgrN3lKYWJwR0F2eEtGVUl6YnNrWFE2SER6eTA0MUFLCmxFazMzUUtCZ1FDYjFrNUJNQkJk" \
    "cVVpd1ZNME9TOEw4QjRJa3BLaFUwTmZHRDZMdk56VFVmcTQyZnBLZlZXYW4KVURvOEFDR1pjaDEy" \
    "TWNaV1JXQ2MwczBRM2twdmN6S3h4S1krRzdUYmQ4ZjlqR1ptOHB6YzM4d2ZLZmpTUDgvTwpxSHpm" \
    "NUx3SWtYTXZ3cDJOOWNEVWtyVWx0L3A0MmJPZzJhOGVqeitibS9weGROVXRaY3FXdXc9PQotLS0t" \
    "LUVORCBSU0EgUFJJVkFURSBLRVktLS0tLQo=" \
    /* end */

// deft/lms/client.crt.base64
#define OEM_LMS_CERT \
    "LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSUN5akNDQWJLZ0F3SUJBZ0lKQU9sZUlpUElp" \
    "dFk4TUEwR0NTcUdTSWIzRFFFQkJRVUFNQmd4RmpBVUJnTlYKQkFNVURTb3VZWFYwWld4aGJpNWpi" \
    "MjB3SGhjTk1UVXhNakEwTVRreU5EUTFXaGNOTWprd09ERXlNVGt5TkRRMQpXakFZTVJZd0ZBWURW" \
    "UVFERkEwcUxtRjFkR1ZzWVc0dVkyOXRNSUlCSWpBTkJna3Foa2lHOXcwQkFRRUZBQU9DCkFROEFN" \
    "SUlCQ2dLQ0FRRUF3UE5OSmNtMU5WN1hrc1M0Y3ZaN3huRmx3Tm1JU3JSeVlqUmxQRnRMdkJjaHRD" \
    "eWsKQVJyS2drbkZEV1NxNmJsdWdZSUp2RmNlNjMyQkp1UFluSGl0VGZ3RTN5V1hLVXRKd0Y2Wi9P" \
    "ZHllT25SN3FVNQpNc24wU1JxcFhXVkIwTEFzajRZQWRTY2tzbXNYejM4OGQ3VTFXdDN1cWNUTnNt" \
    "bGZFQlB1VFpZUVNoTVkxUEdzCmdneXRmckxOeituQWprT2lENlVuL1d3RXVaNUNHZzlEM2RQVTNr" \
    "Ukw5eEZCTHpkbVRKU0RUU3ZOZUxuRFR5dVQKWHNKa0piZ1Zla21QMjFpSFJBTFBMWjJkbG5xZ1Jz" \
    "ZkZSYTNKck1hdjNDSVpIek9jZndQZm4yTWovRHExeWRDWgpqQWVSMmJNdmZzMDRtMXNsYUw5eFhP" \
    "dFplM1o1UmgwL1dMN2xud0lEQVFBQm94Y3dGVEFUQmdOVkhTVUVEREFLCkJnZ3JCZ0VGQlFjREFq" \
    "QU5CZ2txaGtpRzl3MEJBUVVGQUFPQ0FRRUFoS0xlUFZNUUlRVmhYemVJSFBYdXI2TnUKMnE5amlQ" \
    "d0hKUTBYRFQ3TnNsbWJCVzdabjlsbU5TRitIOUpQVVN6bTMwS1p0NkxEVVdsRDlnSG4yN21lSW5N" \
    "UAp4NjlQVlA4eDZtaUZMZ1pDM3k4azhkN05JUVJwckxuVUlCUGd1emxRUGJlMkpjSk81NU9zK2Ew" \
    "RDczOUJ0ZzZICk1kdGUzOTVSVTd6TllRZjNmTitUdlhMYzZ2R3ZtUVlyV1ByeWx5OUdPNmNCd0tT" \
    "Z3lMNmdzOVVaamtFSkFHSTMKRnRPbEVBY2dDeGJxWURTYnFBSjM3YTFpeWUxWXpCNHdGZTA3bzRw" \
    "c3RkY1FLaFNYSFBsSVhSMDlBS1c3VWc3ZwpMK2FqcW9yVlNJYktiRTFoWjN0YW1LaGlBbzdWSCtU" \
    "Y2w1enZaSmRRZkFZelRLUHZoZ09JTWF6TVc5VXYzQT09Ci0tLS0tRU5EIENFUlRJRklDQVRFLS0t" \
    "LS0K" \
    /* end */

#define OEM_CERT_LMS_INITER {       \
    [OEM_CERT_CA]   = OEM_LMS_CA,   \
    [OEM_CERT_KEY]  = OEM_LMS_KEY,  \
    [OEM_CERT_CERT] = OEM_LMS_CERT, \
}   /* end */

// deft/lss/ca.crt.base64
#define OEM_LSS_CA \
    "LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSURMRENDQWhTZ0F3SUJBZ0lKQUxEbFY4dlFy" \
    "RnlhTUEwR0NTcUdTSWIzRFFFQkJRVUFNQmd4RmpBVUJnTlYKQkFNVURTb3VZWFYwWld4aGJpNWpi" \
    "MjB3SGhjTk1UVXhNakEwTVRreE16SXpXaGNOTWprd09ERXlNVGt4TXpJegpXakFZTVJZd0ZBWURW" \
    "UVFERkEwcUxtRjFkR1ZzWVc0dVkyOXRNSUlCSWpBTkJna3Foa2lHOXcwQkFRRUZBQU9DCkFROEFN" \
    "SUlCQ2dLQ0FRRUFxWG9jbmkvYUNEMWxaTmFESFpuUGlVOGJiOVpCVzBEK0lnZVppM0gvOHF5Q2I4" \
    "SkoKQ1hkSmhhcTdaaTlObDJ3VW5TUjlkeTN2QWxkUVhqaElrbXd4aE5oUWdVQXB0Q1hhWGFQRkcv" \
    "VE85Ym4rRC9pTwovSnlvek4ybDNNRkhvKzlqZnlpZlNrWE04OFZtSHNYUndEd29wM2gvTFV5R0s2" \
    "b3FIYnFnWmRwR1pWNGhESUNtCklvbGdPcUVGejR6WlZTeGcrVTBONk5oZURoSG5qSGdoVEhxUjhz" \
    "RjFkWTBnY01hc3gwVWU4ZWVOYk93cm9SdHAKTnI0VGxaWHczTnN5Njg1ZWpLZnBMVFVTdkVZMDV2" \
    "cEVPVEZRRDVkbkpkN052NktTUk52eTVzN1JnRFpxYXBORApQK0wwTW1TTHZEL0U5Vi93cGRnRmYy" \
    "Q2hhZU5vZy9hbnJlck5od0lEQVFBQm8za3dkekFkQmdOVkhRNEVGZ1FVCnRQSnZtbXlYeThaOUdq" \
    "Y2YybFNBQTlwNko0RXdTQVlEVlIwakJFRXdQNEFVdFBKdm1teVh5OFo5R2pjZjJsU0EKQTlwNko0" \
    "R2hIS1FhTUJneEZqQVVCZ05WQkFNVURTb3VZWFYwWld4aGJpNWpiMjJDQ1FDdzVWZkwwS3hjbWpB" \
    "TQpCZ05WSFJNRUJUQURBUUgvTUEwR0NTcUdTSWIzRFFFQkJRVUFBNElCQVFBZTc2Yy9Lb1JZMTRT" \
    "d1MvcTRYL1dBClhXa0dpYW93elgvcWdwMm5VNzh0SXpYUzAvbG9HYTN0czlibFl4Mi82N3YzdlM5" \
    "ajBiK0RuRVB6V2FMYkUyTTIKd050ZVQ5SmRUSFNlRWduVUUrYkhhTExpeEx2WWNMT1BsbTVsOFUz" \
    "WXpLT2tQaHlram1RMzRqbWJzN0ZxU3lIWQo3d0J2VTdDbmg1d1hvT28zNUQwMnRVcm5ZSEtkRGcx" \
    "aGV5UXFGTzhnZkh1d2JodjBOTDJ0azBLK283cFZBQy93CldRVmNTTm5ycDlZUUI1OERNeS9UY2Ji" \
    "SWNXcDV0bzFPZkNjNm5nUHFXTkU3b2x2Vy9wOEk3UTVza1JxTmRJNngKeEtKWHFZQVRieHZyUWRa" \
    "VXZUNVV5WkdXajJ0bVhXVU5aOWpJWGRLZC9YT2RraXlHYVhjc0VwSDlKOHY1WVE2YQotLS0tLUVO" \
    "RCBDRVJUSUZJQ0FURS0tLS0tCg==" \
    /* end */

// deft/lss/client.key.base64
#define OEM_LSS_KEY \
    "LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFb3dJQkFBS0NBUUVBMVFBMCtTK0N3" \
    "blowNTZ1TWw4cE5wcE1YNHlzM2JRSTBvRnRUdUhuc2lxWUh4a09LCmlJM2lSUExvclpacHV6d3Rs" \
    "aEZLbUNyZXYxV05UNi9IcFB5T1hzRnNZclcrUmNkNWVxcjBKVHVUQ0tsdWVTVzAKQytEZDh5QkFJ" \
    "WVIzWkFCL1FmRzZCaldlU3NXVjFySVVsTm0rQSt3WThZdTJvQ25wT2owV08zdjF0MXNYZ043cAph" \
    "c245ejVXK2NDRitPaWJjVXFlcEI5Z1l2NVBsUkMvRmVIUlBhbFc1c2ZuU0lWTWRzSjlQQit2WEdz" \
    "VXFwd2IvCko5N1RqZUREU1FSMXptYk5MRG1KZ2VnTVlvY1ljUGZaZENKSVRjdUwyV3FNVTBWN0dz" \
    "cnVWVytyL3dQTG51ZVoKY3dHaHdFczNPZVY5N1lETUFUaVVZd2JTUXpBS1JSNENxaUZvb1FJREFR" \
    "QUJBb0lCQUVINjMyMzFMcThvWEl4TApFckVPWk1GZHZUK01hbzNrNmdKbVorZTBOUGE1K1p5TDNZ" \
    "VHhWbzcvSTk1cHduVFNibGlYSUtGRVhsMnNsYXFDCmkvaHFsM3Q0dFNkYW53Y2toRmZnVHVLZHNx" \
    "MHpOdHBtT0lDWmRvZFNqU3NVeXByNm9IRC8vUnpoVFJoaVhrajAKUHZYaEFVQmJ5VnBCYWgvdys2" \
    "TWp1eGhRRE14djVUaUZhT3AxZDkrOWlZcEV5dkZvY00yUzh4OVhiME52STRrZApZZzJ3WHZNTUpM" \
    "OUVDYnhGenRrK2wyelRHQ01DOW9wRnQvUmlyZmFzMkJ5d2R1Y1NCbFZmMFdFL1BxVGVvSnlaCmtN" \
    "MjhNdlIyaGllN1Z3bWdZWFFwdk5iYUZaSXc2N2phNmkvUm9qemxZSjhMVTlUbGJpZlBXNXBLSGsr" \
    "U0JCekYKamhSY3lIRUNnWUVBK2F2d25XWWx2Z3FsSWxqVlY1Qk1pM1BvelkrOUQzWXhGZWFuNzVz" \
    "WTErd2c2elNRekVQUgo3QmZHdG9lOHgzL3RDcDc0dlR4QkZxQTFvN0I5SnlFSXZQcXc4U0EwUGtW" \
    "T25wY0lidnVCUmtwT3JEM1FkbkdNCmdCbi9uS2hXT0VGU2pDODJ5OFZ6eTZDUFhmOGphQ05OZmNQ" \
    "ZTFQMDUxVmVud095SDFBYkdzL1VDZ1lFQTJtWlIKbFpNKzVZY29RVXpGOEJWOUg2MXVHdlU2MGFv" \
    "bTFoSWZYc2tXV3MvazRybGY0V2U3SXRya2tTVEtnZkM3TFg4SApvbC9tU3ZQcHRlcGNrRmd2cy9o" \
    "T2R4bzdmaEFqOE1ORnZZZnBtSjhucFZVeFhFd3ZjOVBNYmNCdWpabmZoMFJVCkpKMjdBV2g4cWpO" \
    "aHZxQU53Nnp4eStMZlZBa01lOEJ5MW01QkluMENnWUJFY0tLVHN3V1RrNzF5cmVGRmtKTzcKQW5v" \
    "RnAycFBDN29tNDhTVFJQUEpkTHdOWGFwM28vdXVlVklLaFNqMWt4YTZlNXQ1M0VPSFhVRjI3cEZR" \
    "SVprUQpuTFBXVlZxYzIwalM1bG4wZWxNVW5wZjBWVG1JemxXU1pBNFN0bjUwQzB3Q2k0cTh6eEtQ" \
    "OWlKMnhOTEM0REtFCjBDV01ReUlRWG1mbzFhM2lMRDFaNVFLQmdDN2wwOVVUdG1qaFdoS1RvdG1J" \
    "TThNdW9ic21OOGc0Q25qamtWcncKUVYyazBOaWl4K1VHT2xna3V2b3o5VXhyM2pYSjFtRFd5UVFG" \
    "Vm9WYTJ4K2Y0RFo4TnBVNnk5V3BMNTZPd0JZUAo3bWg0bkxIVEZuTmtaTGd4bmU5ZmFCMytFZExo" \
    "S0ZSb2puU2g5RXMwRjJRd3lHUGhhbGJHMUFBZXBWQ1pwYzlJCkE3S2RBb0dCQUtwKzNvdnYrU3Zl" \
    "eDVSNTUzNEt3Y0NxbU1iMlRXSWZPZVpMZFhCaU5TYXVMM3FDQjVnVVVjTEcKRElxRTRUTUIxY1JM" \
    "THV6RzByVmZ4d2hNSlQyKzBPU3M0YW1ISTlyNzl4V29KMFovRy9IekJ2OWtPRWZWK2tPYgpmWmJm" \
    "bVFGOWF2c0o4dDYwL0xHb1VUSkZXdzE2TDNpYVVWWDk5L1k2d1daVE1raDBuelRECi0tLS0tRU5E" \
    "IFJTQSBQUklWQVRFIEtFWS0tLS0tCg==" \
    /* end */
    
// deft/lss/client.crt.base64
#define OEM_LSS_CERT \
    "LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSUN5akNDQWJLZ0F3SUJBZ0lKQU52TTI0WHVX" \
    "REVtTUEwR0NTcUdTSWIzRFFFQkJRVUFNQmd4RmpBVUJnTlYKQkFNVURTb3VZWFYwWld4aGJpNWpi" \
    "MjB3SGhjTk1UVXhNakEwTVRreE16STBXaGNOTWprd09ERXlNVGt4TXpJMApXakFZTVJZd0ZBWURW" \
    "UVFERkEwcUxtRjFkR1ZzWVc0dVkyOXRNSUlCSWpBTkJna3Foa2lHOXcwQkFRRUZBQU9DCkFROEFN" \
    "SUlCQ2dLQ0FRRUExUUEwK1MrQ3duWjA1NnVNbDhwTnBwTVg0eXMzYlFJMG9GdFR1SG5zaXFZSHhr" \
    "T0sKaUkzaVJQTG9yWlpwdXp3dGxoRkttQ3JldjFXTlQ2L0hwUHlPWHNGc1lyVytSY2Q1ZXFyMEpU" \
    "dVRDS2x1ZVNXMApDK0RkOHlCQUlZUjNaQUIvUWZHNkJqV2VTc1dWMXJJVWxObStBK3dZOFl1Mm9D" \
    "bnBPajBXTzN2MXQxc1hnTjdwCmFzbjl6NVcrY0NGK09pYmNVcWVwQjlnWXY1UGxSQy9GZUhSUGFs" \
    "VzVzZm5TSVZNZHNKOVBCK3ZYR3NVcXB3Yi8KSjk3VGplRERTUVIxem1iTkxEbUpnZWdNWW9jWWNQ" \
    "ZlpkQ0pJVGN1TDJXcU1VMFY3R3NydVZXK3Ivd1BMbnVlWgpjd0dod0VzM09lVjk3WURNQVRpVVl3" \
    "YlNRekFLUlI0Q3FpRm9vUUlEQVFBQm94Y3dGVEFUQmdOVkhTVUVEREFLCkJnZ3JCZ0VGQlFjREFq" \
    "QU5CZ2txaGtpRzl3MEJBUVVGQUFPQ0FRRUFxRnZZV2JpSmFvTmZIRHhReHlQYVkwOVkKeTVOUnJx" \
    "cWNPMUFRejdiMWJqMFNBWDJMNDZYN0hMY0JyYy8vdklkVXFQRGd4RFJzZGF2amVYTmdGbUlVTVBY" \
    "MQpGdlZwdmVEVXpsMU1nYlh4MkIzcjhhUE1FM3M4YVhXZFIyWmVmUnlpK2ZmY3JoeTd3cms1T1o4" \
    "WDhnUXFaZjJTCm02azNlWjZSWkxya3FxREpqaURreE1OcFZyRjY3cVg4Z2FRMk5PV3kvdzd0N1RM" \
    "dUFadHhvNG5LR3ZOUGlRK2UKNU16c3VvOTdKMGtSNlY0Y3E4TDZzV0FwbWVBQ1RJaUVTbzhXYzNt" \
    "QUVCTUVqYk0wZ0RmVGlnTFhnTXk4SWpyeAphMkdMSlVBZ1l3YjJHS1FGc2l2SmdJbm5Sd01oYWh2" \
    "MCsrRk4zdHdVbUVWdXZBMExHZWhnM1NsWXVVWVFuQT09Ci0tLS0tRU5EIENFUlRJRklDQVRFLS0t" \
    "LS0K" \
    /* end */

#define OEM_CERT_LSS_INITER {       \
    [OEM_CERT_CA]   = OEM_LSS_CA,   \
    [OEM_CERT_KEY]  = OEM_LSS_KEY,  \
    [OEM_CERT_CERT] = OEM_LSS_CERT, \
}   /* end */

// deft/ums/ca.crt.base64
#define OEM_UMS_CA \
    "LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSURMRENDQWhTZ0F3SUJBZ0lKQVBiaHJVa25q" \
    "MnhxTUEwR0NTcUdTSWIzRFFFQkJRVUFNQmd4RmpBVUJnTlYKQkFNVURTb3VZWFYwWld4aGJpNWpi" \
    "MjB3SGhjTk1UVXhNakEwTWpFeE56UXpXaGNOTWprd09ERXlNakV4TnpRegpXakFZTVJZd0ZBWURW" \
    "UVFERkEwcUxtRjFkR1ZzWVc0dVkyOXRNSUlCSWpBTkJna3Foa2lHOXcwQkFRRUZBQU9DCkFROEFN" \
    "SUlCQ2dLQ0FRRUF0dnRKUzJUb3ltVjJuZStXK1VQT0hyZ3ZQeWUxYXJyVndncElOU050OVRLZEJo" \
    "SmUKZlFlN3RXWUlDMTBMOG9LcTRoU3dwU1N4ejNzZGpxKzVhQVRHa294a0JESDZic0xyQ0dGNEpz" \
    "L3JPZE5ra2xYMwpDRGlpZlJXRmhNU1dKRitpNllyL0Z5WkJkbGVTekZFOWF0QUw1Zm5TMzExM1or" \
    "ekNvR3UzcnhEZldNT0N6NG8wCkpha0cyM1dMNStGTnFROVBqSlNSLzV6RHhleWZkK0NtRTY5VWx0" \
    "NENua0M3S2hyQ0R0aEllRVNWb1M4TjlqeFoKc3pqcDgrY1kxeGVxempSMFJSMCtXa1p2ZGJtL2Jk" \
    "UWhWdElENlNYVDlabEFKbkFodXJyY2pJcTg3SU0xZ244egpsS3p2NGFBU2p5Nk5GaDRmSlVoQTZ4" \
    "MEhNT1dWbTRWYk9LeW94UUlEQVFBQm8za3dkekFkQmdOVkhRNEVGZ1FVCnRXWGtKK2pCTUZyUTJI" \
    "TTJadVZKY25XTkZCOHdTQVlEVlIwakJFRXdQNEFVdFdYa0orakJNRnJRMkhNMlp1VkoKY25XTkZC" \
    "K2hIS1FhTUJneEZqQVVCZ05WQkFNVURTb3VZWFYwWld4aGJpNWpiMjJDQ1FEMjRhMUpKNDlzYWpB" \
    "TQpCZ05WSFJNRUJUQURBUUgvTUEwR0NTcUdTSWIzRFFFQkJRVUFBNElCQVFBeTAzZ2Q5SS9OSHdh" \
    "Z3M2TGtHa2lxClBjU1l5TEV4ckVZcW1JdHpqSHVpaXM0c0FlQVMyY0NONm5udFpsYzhEODR0Rmk0" \
    "YlRURUU4WEdocG1GUFdYd3kKZVQ2NlZWMElSSVZZc2swWDY4VDVKMjl2QnZoOWs1Si9KdzU2VFRS" \
    "aGR5K1hwOHYyQXgxVEYwM2JqREt0NkJWZAp4RUdSaGJ3R25SMlBtYjk5WVgzeFJONUFWUno2VjNC" \
    "dWJLTUhFL1l0UWNUamJrRlJXcjlXUVZFTGcrNDVUOVlXCjlrVnN6cW5Tb0haWVZESkFMRnJWUVQr" \
    "L3F0QWVRNDVJcXN2TlNoODVkeXRqdnYwNktzUWgwS3BMNVlRYUZKa3IKRUNOZnlxSU5wY3JRK0tj" \
    "OTM3bUdEZ1hHMEQ4YlRuYzF2NXdUNEpTeE9QMzZ1ZisxelZrV2xwUGEwWm1DNHBCTwotLS0tLUVO" \
    "RCBDRVJUSUZJQ0FURS0tLS0tCg==" \
    /* end */

// deft/ums/client.key.base64
#define OEM_UMS_KEY \
    "LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFcEFJQkFBS0NBUUVBekIvRTFic0pm" \
    "OEJhcXo3clNURnllRmI4WVhPcXBsVm5lSFBRQVcrckZxUHg3MHBlCk1HTzB4Vjlab3VJNG5FVXZ1" \
    "WGIrTzR1M1NyMmlFL2RpQ201aEFvYXh3dWkzT0tnRGorWTlJMW5ZWFpKa0VCTHYKZlY0MENGSm1P" \
    "VVR4KzlGUy9GMm1sdXROOE1HK1B0ZE5sUXdyelQ5UlhTajI4UzFtbzlpbityMkhmQXRlN2NLNwo2" \
    "d21pcnVmcFVMQWwzTGFHR0Y4VGg5NVlaKzFkVXBqWmNKU0pjY0ljTVk5T2pQL2JMZ2pBUmJJMFhr" \
    "TnoyNEpHCkdIbldoSFlEL0FrWUZiRzFLcTNMbDlEWncrQTQ4RkJIK243NTlpbXJTa0ovWnVnRXVP" \
    "Z05sR3pVWTZGc0pwTEcKd21DaEExU0xhUmlsc053dWpYQWRmQmtrMDRSOTdTTDB3R0o4eFFJREFR" \
    "QUJBb0lCQUVOTGZmTVFsSk1mY1VUZgptMlluL1JzYllsMG1hdTRuWGROaUt3TXVNM05sWFUrTWJ2" \
    "ejVBaEhIMjU5TmdXaFRqdVpRQ29UTERRU0EvRHdrCjVKZVh5RGV5L0V2THVRdmVxRDM2bzlGUnYz" \
    "a3AxLzlNbnRQd0NLdHRydkhXZndpTC9CWGpNaGF6UUtETTN0R00KM2JEenNFVWNTTkN6dnFYSVYv" \
    "eFFZeURROXllM2FUdlJjekNmaVltc2t2WENIUG9hR0ZxS3U1aDlIR2ZJQVdLNQpFdlRqcW9HaXU3" \
    "QXU2bi95djBZYjFDOUJ2WExta1doSU9ySjRaaVgzM0xlRHpydXJEcTVnNEtPNTc3STZLOTRPCndP" \
    "amVDZm9oNml0YmhuNnNPbVQwdU9IZmdqbStpbGZ4ZmNER1lUblVWd1RtYnRtL2d4SlhOZ2xRUHZJ" \
    "TUl5dTIKZndQbjg0RUNnWUVBNWJSNGk3NFVVV040RmpvbS9RRWhlWlJZUTB2Si9ySk1OazFMbUx4" \
    "NExqTWVKekVJTVRIdgpNbXUyZEcrVmhtWlpOWnBqV3h1N3FITnJycm9Qa29pM1lZL2NZaVJ3TzZE" \
    "TUdJaCs5ZTFLZ1NLM3EzTFBneUlJCnhaUGI2bjlTUDR2V3dBSVAxOW9TZFVZUE94MUw3M3g4eGNs" \
    "Um5LTXNlbjVjZFhWUDNHbkVNaEVDZ1lFQTQzMmwKM0VOa0RXTllpK1hmZEsrY2doSFJPNHRpcUto" \
    "Z3dPYTBEZjM5c3hNU0NjWUpRRXg2QUl2NGpVQlN5NkV1Ny84VQp5YU1iZmV2b0oxY1BGVWVxanRT" \
    "ZTJwam9KZjJkYzBXTFZOb3lrUEtWcVBQdnpNY1VSS2J4R2IwY25rSnhmOGFnCkc2VDY4bEdZbjJm" \
    "MVZPRGNHQUlQWVV6ZkEwT3oxT2VudnBHaTYzVUNnWUVBdjNWeU9IL2hiWWtzRWRLOHBvc24Kdksv" \
    "QjlhMlpHdHJBSFU3RjRQUE9kQ3VWU1hOYjhhMCtRMWQyUjR5dUk4enhOSFdQdE9QandWOGE5Uk1P" \
    "WWNQawpuY1FNSEIxbEVyNU8vMi9kNnR1ekN2bkYvM2g5c0Q1UTgyWHZtb0F2TE5wQXo5K29GdkVr" \
    "QzIydXlrWDhBZ3hQCjc0ZUNwNzEyR0U5MGYwNmRsd2p0U0JFQ2dZQW1rMExSdzlldWNYOHRVZEIv" \
    "Yjl2aExuWkZXNUJpT3hNVkFzbmIKYVl0Z2wwWkN4QU1EbFRSRzdYVTdOb0lUcDRSWEMvRVJkQWdR" \
    "WlRWYm5tYWZYQ1N1elowV1FrRnpWN3NnTmRXMwprTHhzV2NaSS9QeFh4QlpLd2VnMlRod1JOZ0Vi" \
    "QXFwWnBiTXI3ZXdzYzZ0bE5pLzhIalZQY1lLTGdpcDNXc1RXCkNzb09sUUtCZ1FDd3lqOGZsdEMy" \
    "K1ZnejE4bCszV2dzRi9kWXV1eElMckc4UHhjR3NCbXpESThtUktoVFRvYkwKZWxmQjd5RHkwNG9D" \
    "ZXRXWllZQlBPdmhmR1dXUUpuQUw0K0o1TUZheE5YbTJRUVM0NTg1WG9SbllrcXlIWEZpMwpYc2Ru" \
    "SEsvTUlnQUFLOEUwaXBTNnlkN3c3SVNqQnlzNWFFaER1ZGFKZ085aFNpSGdQblFCd3c9PQotLS0t" \
    "LUVORCBSU0EgUFJJVkFURSBLRVktLS0tLQo=" \
    /* end */

// deft/ums/client.crt.base64
#define OEM_UMS_CERT \
    "LS0tLS1CRUdJTiBDRVJUSUZJQ0FURS0tLS0tCk1JSUN5akNDQWJLZ0F3SUJBZ0lKQU1BNUJLamtR" \
    "ZVN3TUEwR0NTcUdTSWIzRFFFQkJRVUFNQmd4RmpBVUJnTlYKQkFNVURTb3VZWFYwWld4aGJpNWpi" \
    "MjB3SGhjTk1UVXhNakEwTWpFeE56UTBXaGNOTWprd09ERXlNakV4TnpRMApXakFZTVJZd0ZBWURW" \
    "UVFERkEwcUxtRjFkR1ZzWVc0dVkyOXRNSUlCSWpBTkJna3Foa2lHOXcwQkFRRUZBQU9DCkFROEFN" \
    "SUlCQ2dLQ0FRRUF6Qi9FMWJzSmY4QmFxejdyU1RGeWVGYjhZWE9xcGxWbmVIUFFBVytyRnFQeDcw" \
    "cGUKTUdPMHhWOVpvdUk0bkVVdnVYYitPNHUzU3IyaUUvZGlDbTVoQW9heHd1aTNPS2dEaitZOUkx" \
    "bllYWkprRUJMdgpmVjQwQ0ZKbU9VVHgrOUZTL0YybWx1dE44TUcrUHRkTmxRd3J6VDlSWFNqMjhT" \
    "MW1vOWluK3IySGZBdGU3Y0s3CjZ3bWlydWZwVUxBbDNMYUdHRjhUaDk1WVorMWRVcGpaY0pTSmNj" \
    "SWNNWTlPalAvYkxnakFSYkkwWGtOejI0SkcKR0huV2hIWUQvQWtZRmJHMUtxM0xsOURadytBNDhG" \
    "Qkgrbjc1OWltclNrSi9adWdFdU9nTmxHelVZNkZzSnBMRwp3bUNoQTFTTGFSaWxzTnd1alhBZGZC" \
    "a2swNFI5N1NMMHdHSjh4UUlEQVFBQm94Y3dGVEFUQmdOVkhTVUVEREFLCkJnZ3JCZ0VGQlFjREFq" \
    "QU5CZ2txaGtpRzl3MEJBUVVGQUFPQ0FRRUFjQW14eEdrSUpKWDluZjl1WTM1dUVCY0EKZXlSTWdy" \
    "N0dXaUVmc1NsUnc4YjcwK0pFZld2ZStyUDQ2ZktkZ2d6K2hqb21hbGR0MmFXNjJYRmRDVkdNVm1S" \
    "OApsMDZMUXVaRnEyTnZiMmliYS8yM1VJaHhNNWdWNHNpZGhmYXZFQWthcFBrbVVJRldyMHpxY3M4" \
    "dDlHTlpucDUyCmF3U0gxdlZHN2MyVkdxbTR0L2drYWpqZUtzaC9pUUtuLzRqa2pjTWFWN0l5TGNW" \
    "QjBoeWJxZ2ZlcXEwNTlCM2gKTjFib2JveHhpVzJva3hWeWxtNTZ3cHNhenlCVE1nQmMrSUVFb3ho" \
    "TldHLys0NTFUVUllbVpkeFpZSHhzN3RDbwpiZWEzUWlVRjJwMGU2OXg1SDYxVWlRNVRxcXhXdito" \
    "TGlxaUlTYjNqeVgrUGRVV0plQUNDenFBYnlRakJyQT09Ci0tLS0tRU5EIENFUlRJRklDQVRFLS0t" \
    "LS0K" \
    /* end */

#define OEM_CERT_UMS_INITER     {   \
    [OEM_CERT_CA]   = OEM_UMS_CA,   \
    [OEM_CERT_KEY]  = OEM_UMS_KEY,  \
    [OEM_CERT_CERT] = OEM_UMS_CERT, \
}   /* end */

#define OEM_CERT_INITER                     {   \
    .cert = {                                   \
        [OEM_APP_LMS] = OEM_CERT_LMS_INITER,    \
        [OEM_APP_LSS] = OEM_CERT_LSS_INITER,    \
        [OEM_APP_UMS] = OEM_CERT_UMS_INITER,    \
    },                                          \
}   /* end */

#define OEM_INITER __OEM_INITER( \
    OEM_BASE_INITER,    \
    OEM_RSYNC_INITER,   \
    OEM_LSS_INITER,     \
    OEM_CERT_INITER)
/******************************************************************************/
#endif /* __LIB_OEM_DEFT_H__ */
