#!/usr/bin/env bash

MEMUSE='./build/memuse'
FAILED=0

echo 'Running memuse integration tests...'

#$MEMUSE -h
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '1:0|2:0|4:b'
#if [ $? -ne 1 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '1:1|2:1|4:1'
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '1:0|4:0' -m
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 1
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 2
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 3
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 4 -d
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 5
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 8 -s
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '4:0' -p 9
#if [ $? -ne 1 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '16000:0' -s
#if [ $? -ne 1 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '16000:0' -e
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
#
#$MEMUSE -c '16000:0' -e -s
#if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi

if [ -n "$GITHUB_ACTIONS" ] && [ "$(uname)" == "Darwin" ]
then
  echo "skipping timeout tests"
else
  timeout --signal TERM --verbose --preserve-status 2s $MEMUSE -c '1:10' -w
  if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi

  timeout --signal INT --verbose --preserve-status 2s $MEMUSE -c '1:0|2:10' -w
  if [ $? -ne 0 ]; then FAILED=1; echo "ERROR"; fi
fi

echo "FAILED: $FAILED"
exit $FAILED
