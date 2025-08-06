#!/bin/bash

# E.g.: ./build.sh -d --sqlite

cc="g++"
options="-I. -m32 -fPIC -Wall -fvisibility=hidden"

separator="---------------------"

while [[ $# -gt 0 ]]; do
    arg="$1"
    case $arg in
        -d | --debug)
            debug="-g -ggdb -O0" # debug build without optimization
            ;;
        -u | --unsafe)
            unsafe=true
            ;;
        -l | --libcurl)
            libcurl=true
            ;;
        *)
            unrecognized_arg=$arg
            break
            ;;
    esac
    shift
done

if [ -v unrecognized_arg ]; then
    echo "Unrecognized argument \"$unrecognized_arg\", aborting."
    exit 1
fi

echo $separator

echo -n "Debug: "
if [ -v debug ]; then
    echo " ON"
else
    echo " OFF"
    debug=""
fi

echo -n "Unsafe: "
if [ -v unsafe ]; then
    echo "ON"
    constants+=" -D ENABLE_UNSAFE=1"
else
    echo "OFF"
    constants+=" -D ENABLE_UNSAFE=0"
fi

libcurl_found=0
libcurl_link=""
libcurl_libpath="/usr/lib/i386-linux-gnu/libcurl.so.4"
echo -n "libcurl: "
if [ -v libcurl ]; then
    if [ -e "$libcurl_libpath" ]; then
        libcurl_found=1
        libcurl_link="-lcurl"
        constants+=" -D COMPILE_LIBCURL=1"
        echo "ON"
    else
        echo "requested but lib not found, aborting."
        exit 1
    fi
else
    echo "OFF"
    constants+=" -D COMPILE_LIBCURL=0"
fi

echo $separator

mkdir -p ../bin
mkdir -p objects

echo "##### COMPILE CRACKING.CPP #####"
$cc $debug $options $constants -c cracking.cpp -o objects/cracking.opp

echo "##### COMPILE GSC.CPP #####"
$cc $debug $options $constants -c gsc.cpp -o objects/gsc.opp

echo "##### COMPILE GSC_ENTITY.CPP #####"
$cc $debug $options $constants -c gsc_entity.cpp -o objects/gsc_entity.opp

echo "##### COMPILE GSC_WEAPON.CPP #####"
$cc $debug $options $constants -c gsc_weapon.cpp -o objects/gsc_weapon.opp

echo "##### COMPILE GSC_PLAYER.CPP #####"
$cc $debug $options $constants -c gsc_player.cpp -o objects/gsc_player.opp

echo "##### COMPILE GSC_UTILS.CPP #####"
$cc $debug $options $constants -c gsc_utils.cpp -o objects/gsc_utils.opp

echo "##### COMPILE GSC_BOTS.CPP #####"
$cc $debug $options $constants -c gsc_bots.cpp -o objects/gsc_bots.opp

echo "##### COMPILE JUMP.CPP #####"
$cc $debug $options $constants -c jump.cpp -o objects/jump.opp

echo "##### COMPILE VCODLIB.CPP #####"
$cc $debug $options $constants -c vcodlib.cpp -o objects/vcodlib.opp

echo "##### COMPILE QVSNPRINTF.C #####"
$cc $debug $options $constants -c vendor/qvsnprintf.c -o objects/qvsnprintf.opp

echo "##### LINK    vcodlib.so #####"
objects="$(ls objects/*.opp)"
$cc -m32 -shared -L/lib32 -o ../bin/vcodlib.so -ldl $objects -lpthread  $libcurl_link
rm objects -r
