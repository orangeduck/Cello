if [ `which libtoolize` ]
  then 
    libtoolize
fi
if [ `which glibtoolize` ]
  then 
    glibtoolize
fi

automake --add-missing
autoreconf -i
./configure
make

