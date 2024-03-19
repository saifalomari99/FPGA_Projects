# SoC_Chasing_LED_Function - Saif Alomari

About  FPGA-based SoC:

A “high-end” embedded system usually has a processor and simple I/O peripherals to perform general user interface and housekeeping tasks and special hardware accelerators to handle computation-intensive operations. These components can be integrated into a single integrated circuit, commonly referred to as an SoC (system on a chip). 
As the capacity of FPGA devices continues to grow, the same design methodology can be realized in an FPGA chip. Instead of just realizing the system functionalities through customized software, we can incorporate customized hardware into the embedded system as well. The FPGA technology allows us to tailor the processor, select only the needed I/O peripherals, create a custom I/O interface, and develop specialized hardware accelerators for computation-intensive tasks.

The FPro system is composed of those major parts shown in the diagram down below: 

- Processor module: The processor module consists of a processor, a memory controller
core, and RAM. It is the part that is constructed from the vendors’ IP
cores.
- FPro bridge and FPro bus: The processor needs to communicate with other cores. This is done by
a bus or interconnect structure specified in the vendor’s IP platform.
The modern interconnect is designed to accommodate a wide variety
of communication and data transfer needs and involves complex
protocols.
- MMIO (memory-mapped I/O) subsystem: The MMIO subsystem provides a framework to accommodate
memory-mapped general-purpose and special I/O peripherals as well
as hardware accelerators. 


<img src='./pictures/file_hierarchy.jpg' width='800'>


The steps to build the Chasing_LED function: 

1. The 16 discrete LEDs are used as output, one lit at a time.
2. The lit LED moves sequentially in either direction. It changes direction when reaching 
the rightmost or leftmost position.
3. The slide switch 0 (labeled sw0 on the Nexys 4 DDR board) is used to “initialize” the 
process. When it is 1, the lit LED is moved to the rightmost position.
4. The next five slide switches (sw1 to sw5) are used to control the chasing speed of the 
LED. The highest speed should be slow enough for visual inspection.
5. When the chasing speed changes, a one-line message is transmitted to the console via the 
UART core. The format of the message is “current speed: ddd”, where ddd is the value of 
five speed setting switches
