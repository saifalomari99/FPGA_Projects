# FIFO Buffer

A FIFO buffer, also known as a first-in, first-out buffer, is a data structure used in electronics to manage data transmission between two processes or devices. It operates on the principle that the data that enters the buffer first will be the first to exit.
FIFO buffers are often used in scenarios where there is a difference in the rate of data production and consumption, acting as a temporary storage mechanism to smooth out these differences. 

An asymmetric FIFO buffer is a variant of the traditional FIFO buffer where the read and write operations are not symmetrical and data sizes are not symmetrical.

<img src='./pictures/fifo.png' width='400'>

The design of the FIFO Buffer (8-bit input, 8-bit output): 

<img src='./pictures/fifo_buffer_design.png' width='400'>

The design of the Asymmetric FIFO Buffer (16-bit input, 8-bit output): 

<img src='./pictures/asymmetric_fifo_buffer_design.jpg' width='400'>

`------------------------------------------------------------------------------------------------------------`

Screenshot of the simulation output: 

<img src='./pictures/full_screenshot.png' width='800'>


Zoomed-in screenshot of the simulation output (part 1): 

<img src='./pictures/zoomed1_screenshot.png' width='800'>

Zoomed-in screenshot of the simulation output (part 2): 

<img src='./pictures/zoomed2_screenshot.png' width='800'>
