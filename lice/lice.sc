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

ALIAS sc {
  IF ([$0]==[-f]) {
     IF (!ismset(V)) {
        @ :xum = 1;
        @ lice.set(main.mset ${lice.get(main.mset)##[V]});
     };
     NAMES ${ischannel($1) ? [$1] : C};
     IF (xum) {WAIT -CMD @ lice.set(main.mset $strip(V $lice.get(main.mset)))};
     RETURN;
  };
  @ :chan = ischannel($0) ? [$0] : C;
  IF (chan == [0]) {RETURN};
  UNLESS (onchannel($servernick() $chan)) {
     NAMES $chan;
     RETURN;
  };

  @ :max = fmt.sc_max() ? fmt.sc_max() : (word(0 $geom()) - 1) / 11;
  @ :min = ismset(T) ? ((strlen($strftime($lice.get(main.tsfmt))) + 2) / 11) + 1 : 0;
  @ :max -= min;

  @ scn.v = scn.o = scn.l = 0;
  @ :scn.list = [];

  ECHO $fmt.sc_top($chan);
  FE ($channel($chan)) nick {
    PUSH scn.list $_proc.scn_mode($nick);
    IF (numwords($scn.list) != max) {CONTINUE};
    ECHO $fmt.sc_mid($scn.list);
    @ :scn.list = [];
  };
  IF (strlen($scn.list)) {ECHO $fmt.sc_mid($scn.list)};
  @ :scn.t = scn.v + scn.o + scn.l;
  IF (fmt.sc_bot()) {ECHO $fmt.sc_bot($chan $scn.o $scn.v $scn.l $scn.t);};
  purge scn;
};

ALIAS _proc.scn_mode (nick) {
  @ :nic = mid(2 9 $nick);
  IF (left(1 $nick) == [@]) {
    @ scn.o++;
    ^LOCAL mod @;
  } ELSIF (mid(1 1 $nick) == [+]) {
    @ scn.v++;
    ^LOCAL mod +;
  } ELSE {
    IF (strlen($nick)) {@ scn.l++};
  };
  @ :ret = mod ## nic;
  RETURN $ret;
};
#tjh/09
