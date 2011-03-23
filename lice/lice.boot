#
#   IRC Script Program. For use with ircii-EPIC5 clients.
#   Copyright (C) 2000 SrfRoG (cag@codehack.com)
#
# ---------------------------------------------------------------------------
# All code by SrfRoG, unless specified. Visit http://lice.codehack.com
# ---------------------------------------------------------------------------
# Updated for EPIC5 by tjh

IF (word(2 $loadinfo()) != [pf]) {
  LOAD -pf $word(1 $loadinfo());
  RETURN;
};

PACKAGE LiCe;

# BOOT - Multiple channel kickban
ALIAS boot {
  UNLESS (@) {
    uecho Usage: /BOOT <nick>;
    RETURN;
  };
  @ :nick = [$0];
  @ :kmsg = strlen($1) ? [$1-] : randread($main.reason_kick);
  @ :serv = winserv();
  @ :patt = mask(0 $get_uh($nick));
  FE ($mychannels()) chan {
    UNLESS (onchannel($nick $chan)) {CONTINUE;};
    _proc.ban $chan $serv $patt $nick;
    _proc.quote_add $serv KICK $chan $nick :$kmsg;
  };
  _proc.quote_flush $serv;
};
