# d8b_pi

This project aims to replace the CPU unit of the Mackie d8b mixer with a Raspberry pi type embedded linux device.


Various notes:
using the /dev/ttyUSB* probably demands the user being part of the "dialout" group.
This can be fixed with:
sudo usermod -aG dialout your_username

