# nRF24L01+ to PureData
This is the source code for the [Data from nRF24L01+ to Pure Data](https://calcium3000.wordpress.com/2016/09/21/data-from-nrf24l01-to-pure-data/) tutorial.  All code is tested and ready for deployment!

Note that the [RF24 library](https://tmrh20.github.io/RF24/) from TMRh20 is a dependency for both the transmitter and receiver.

## Transmitter (Tx)
Use an Arduino as the one-way transmitter of a single unsigned byte, which increments every second.  Program the `c3k_nRFtoPd-Tx.ino` sketch and connect the nRF as described in the sketch's header.

## Receiver (Rx)
Use a Raspberry Pi connected to an nRF24L01+ as the receiver and host of Pure Data (Pd).  Follow the compilation and use instructions in `c3k_nRFtoPd-Rx.cpp`, and have the `udp_test.pd` Pd patch open.

## License
This software is released under the [GNU General Public License](https://www.gnu.org/licenses/gpl.html).  You are free to use and improve this software; however, the released improved software must also be free.  See `LICENSE.txt` or the GNU website for more information.