# Building libswiftnav in 2022 is not working.
# Need to checkout submodules.
if [ ! -e libswiftnav ] ; then
	git clone https://github.com/swift-nav/libswiftnav/
	cd libswiftnav
	git submodule update --init --recursive
	mkdir build
	cd build
	cmake ../
	make
	cd ../..
fi

gcc -o ubx_nav_pvt ubx_nav_pvt.c
gcc -o ubx_nav_svin ubx_nav_svin.c
gcc -o ubx_cfg_tmode3 ubx_cfg_tmode3.c
gcc -o ubx_rxm_rawx ubx_rxm_rawx.c
gcc -o ubx_filter ubx_filter.c
gcc -I ./libswiftnav/include -L ./libswiftnav/build -o ubx_nav_sol ubx_nav_sol.c  -lswiftnav -lm


