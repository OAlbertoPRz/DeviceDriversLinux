cmd_/home/oaperez/Documents/ldd/DeviceDriversLinux/InterruptHandler/Module.symvers := sed 's/ko$$/o/' /home/oaperez/Documents/ldd/DeviceDriversLinux/InterruptHandler/modules.order | scripts/mod/modpost -m    -o /home/oaperez/Documents/ldd/DeviceDriversLinux/InterruptHandler/Module.symvers -e -i Module.symvers   -T -