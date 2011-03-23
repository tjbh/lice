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

ALIAS mk {
  UNLESS (ischanop($N $C)) {
    XECHO -B $ch($C)\: You're not channel operator.;
    RETURN;
  };
  UNLESS (pass(: $*)) {
    ^ASSIGN kick_list $*;
    @ kick_msg = randread($main.reason_kick);
  }{
    ^LOCAL mk.t $*;
    @ kick_list = before(: $mk.t);
    @ kick_msg = after(: $mk.t);
  };
  _proc.loop_kick $winserv() $C kick_list kick_msg;
};
#tjh/09
