#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

TITCOIND=${TITCOIND:-$SRCDIR/titcoind}
TITCOINCLI=${TITCOINCLI:-$SRCDIR/titcoin-cli}
TITCOINTX=${TITCOINTX:-$SRCDIR/titcoin-tx}
TITCOINQT=${TITCOINQT:-$SRCDIR/qt/titcoin-qt}

[ ! -x $TITCOIND ] && echo "$TITCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
TITVER=($($TITCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for titcoind if --version-string is not set,
# but has different outcomes for titcoin-qt and titcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$TITCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $TITCOIND $TITCOINCLI $TITCOINTX $TITCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${TITVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${TITVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
