# Ragel machine : SIP URI
# RFC 3261
# SIP URI scheme seems to be somehow not compatible with RFC986.
# http://www.ietf.org/mail-archive/web/sip/current/msg26338.html
# http://www.ietf.org/mail-archive/web/sip/current/msg26385.html
# Here comes RFC3261 ABN form.

%%{
  machine sip_uri;


  ESCAPED         = PCT_ENC;
  SIP_UNRESERVED  = alnum | [\-_\.!~\*'()];
  USER_UNRESERVED = [&=+$,;?/];
  PARAM_UNRESERVED= [[\]/:&+$];
  TOKEN           = (alnum | [\-\.!%*_+`'~]){1,};
  PARAMCHAR       = PARAM_UNRESERVED | SIP_UNRESERVED | ESCAPED;
  PNAME           = PARAMCHAR{1,};
  PVALUE          = PARAMCHAR{1,};
  HNV_UNRESERVED  = [[\]/?:+$];
  HNAME           = (HNV_UNRESERVED | SIP_UNRESERVED | ESCAPED){1,};
  HVALUE          = (HNV_UNRESERVED | SIP_UNRESERVED | ESCAPED)*;
  HEADER          = HNAME >mark_start "=" HVALUE %fetch_sip_uri_header;

  TEL_SUBSCRIBER  = zlen; # will implement in future

  USER            = ( SIP_UNRESERVED | ESCAPED | USER_UNRESERVED ){1,};
  PASSWORD        = ( SIP_UNRESERVED | ESCAPED | [&=+$,] )*;
  SIP_UINFO       = ( USER | TEL_SUBSCRIBER ) >mark_start ( ":" PASSWORD )? %fetch_userinfo "@";
  DOMAINLABEL     = alnum | (alnum (alnum | "-")* alnum);
  TOPLABEL        = alpha | (alpha (alnum | "-")* alnum);
  HOSTNAME        = (DOMAINLABEL ".")* TOPLABEL "."?;
  SIPHOST         = (HOSTNAME | IPv4_ADDR | IPv6_ADDR) >mark_start %fetch_host;

  HOSTPORT        = SIPHOST (":" digit{1,} >mark_start %fetch_port)? ;

  OTHER_PARAM     = PNAME ("=" PVALUE)?;
  LR_PARAM        = "lr";
  MADDR_PARAM     = "maddr=" SIPHOST;
  TTL_PARAM       = "ttl=" digit{1,3};
  METHOD_PARAM    = "method=" TOKEN{1,};
  USER_PARAM      = "user=" ( "phone" | "ip" | TOKEN);
  TRANSPORT_PARAM = "transport=" ( "udp" | "tcp" | "sctp" | "tls" | TOKEN );
  URI_PARAM       = TRANSPORT_PARAM | USER_PARAM | METHOD_PARAM |
                    TTL_PARAM | MADDR_PARAM | LR_PARAM | OTHER_PARAM;

  URI_PARAMS      = ( ";" URI_PARAM )* >mark_start %fetch_sip_uri_params;

  HEADERS         = "?" HEADER ("&" HEADER)*;

  SIP_URI         = ("sip"i [sS]?) %fetch_scheme ":"
                    SIP_UINFO? HOSTPORT URI_PARAMS HEADERS?;

}%%
