#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>

void main(int argc, char *argv[]) {
	int file_i2c;
	int length;
	unsigned char write_buffer[3] = {0};
	unsigned char read_buffer[2] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x08;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}

    //----- WRITE BYTES -----
	write_buffer[0] = 6; // Offset address
    write_buffer[1] = 0; // data byte
	write_buffer[2] = 0; // data byte
	length = 2; //<<< Number of bytes to write
	if (write(file_i2c, write_buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	} else
    {
        printf("Write was successful.\n");
    }
	
	
	//----- READ BYTES -----
	length = 2;	//<<< Number of bytes to read
	if (read(file_i2c, read_buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		unsigned int val = read_buffer[1];
		val = val << 8;
		val += read_buffer[0];
		printf("Data read: %d\n", val);
	}
}