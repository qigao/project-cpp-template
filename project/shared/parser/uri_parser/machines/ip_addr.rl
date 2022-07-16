# Ragel machine : IP address
# IPv4 and IPv6 addresses 
%%{
  machine ip_addr;

  # Ragel machine: IPv4 addresses
  # ===
  # Implements RFC 3986 [Section 3.2.2]
  #      IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
  #
  #      dec-octet   = DIGIT                 ; 0-9
  #                  / %x31-39 DIGIT         ; 10-99
  #                  / "1" 2DIGIT            ; 100-199
  #                  / "2" %x30-34 DIGIT     ; 200-249
  #                  / "25" %x30-35          ; 250-255
  #
  # Additionally also treating leading "0".
  # For example 001.010.100.1 == 1.10.100.1
  OCTET4 =  ("0" | "00")? digit |
            "0"? [1-9] digit    |
            "1" digit{2}        |
            "2" [0-4] digit     |
            "25" [0-5]          ;

  IPv4_ADDR = (OCTET4 "."){3} OCTET4;

  # IPv6 addresses
  # Implements RFC 3986 [Section 3.2.2]
  # IPv6address =                            6( h16 ":" ) ls32
  #                   /                       "::" 5( h16 ":" ) ls32
  #                   / [               h16 ] "::" 4( h16 ":" ) ls32
  #                   / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
  #                   / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
  #                   / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
  #                   / [ *4( h16 ":" ) h16 ] "::"              ls32
  #                   / [ *5( h16 ":" ) h16 ] "::"              h16
  #                   / [ *6( h16 ":" ) h16 ] "::"
  # 
  #       ls32        = ( h16 ":" h16 ) / IPv4address
  #                   ; least-significant 32 bits of address
  # 
  #       h16         = 1*4HEXDIG
  #                   ; 16 bits of address represented in hexadecimal
  H16       = xdigit{1,4} ;
  LS32      = (H16 ":" H16) | IPv4_ADDR ;
  IPv6_ADDR =                        (H16 ":"){6} LS32 |
                                "::"              H16  |
                                "::" (H16 ":"){5} LS32 |
                            H16 "::" (H16 ":"){4} LS32 |
              (H16 ":"){,1} H16 "::" (H16 ":"){3} LS32 |
              (H16 ":"){,2} H16 "::" (H16 ":"){2} LS32 |
              (H16 ":"){,3} H16 "::"  H16 ":"     LS32 |
              (H16 ":"){,4} H16 "::"              LS32 |
              (H16 ":"){,5} H16 "::"              H16  |
              (H16 ":"){,6} H16 "::"                   ;

  # IP address
  IP_ADDR = IPv4_ADDR | IPv6_ADDR;

}%%
