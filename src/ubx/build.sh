if [ ! -e libswiftnav ] ; then
	git clone https://github.com/swift-nav/libswiftnav/
	cd libswiftnav
	mkdir build
	cd build
	cmake ../
	make
	cd ../..
fi

gcc -o ubx_nav_pvt ubx_nav_pvt.c
gcc -I ./libswiftnav/include -L ./libswiftnav/build -o ubx_nav_sol ubx_nav_sol.c  -lswiftnav -lm


