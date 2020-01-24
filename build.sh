os=$(uname)

if [ "$os" = "Darwin" ]
then
    echo "Cannot compile 'reyclr-client' on MacOSx"
    exit 0
fi

make
