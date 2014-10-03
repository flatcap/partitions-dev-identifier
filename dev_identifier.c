/* Copyright (c) 2013 Richard Russon (FlatCap)
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/**
 * main
 */
int
main (int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	 char *memblock = NULL;
	 int fd = -1;
	 struct stat sb;
	 long size = 0;

	 fd = open (argv[1], O_RDWR);
	 fstat (fd, &sb);

	 size = sb.st_size;
	 if (size == 0) {
		 // Might be a device
		 size = lseek(fd, 0, SEEK_END);
	 }

	 printf ("size: %lu \n", size);

	 memblock = (char*) mmap (NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
	 if (memblock == MAP_FAILED) {
		perror ("mmap");
		return (EXIT_FAILURE);
	 }

	 long hundred = 1024*1024*100;
	 char buffer[16];
	 memset (buffer, 0, sizeof (buffer));
	 for (long i = 0; i < size; i += 512) {
		 snprintf (buffer, sizeof (buffer), "%012ld", i);
		 memcpy (memblock+i, buffer, 12);
		 if ((i % hundred) == 0) {
			 printf ("%ld MiB\n", i * 100 / hundred);
		 }
	 }

	 return 0;
}
