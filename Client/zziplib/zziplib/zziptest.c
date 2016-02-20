/*
 * Author: 
 *	Guido Draheim <guidod@gmx.de>
 *	Tomi Ollila <Tomi.Ollila@iki.fi>
 *
 *	Copyright (c) 1999,2000,2001,2002 Guido Draheim
 * 	    All rights reserved,
 *	    use under the restrictions of the
 *	    Lesser GNU General Public License
 *          note the additional license information 
 *          that can be found in COPYING.ZZIP
 */

#include <stdio.h>

#if defined _MSC_VER /* Win32*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define sleep Sleep
#endif

#include <zzip.h>

#ifdef ZZIP_HAVE_UNISTD_H
#include <unistd.h> /* sleep */
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

int main(int argc, char ** argv)
{
    ZZIP_DIR * dir;
    const char * name = "test.zip";
    zzip_error_t rv;
    int i;
  
    if (argc > 1 && argv[1] != NULL)
    {
        name = argv[1];
        argv++; argc--;
    }

    printf("Opening zip file `%s'... ", name);
    { 	int fd = open (name, O_RDONLY|O_BINARY);
        if (fd == -1) { perror ("could not open input file"); return 0; }
        if (! (dir = zzip_dir_fdopen(fd, &rv)))
        {
            printf("error %d.\n", rv);
            return 0;
        }
    } printf("OK.\n");
  
#if 1
    printf("{check...\n");
    { struct zzip_dir_hdr * hdr = dir->hdr0;
    
        if (hdr == NULL) 
          { printf ("could not find first header in dir_hdr"); }
        else
        {   
            while (1)
            {
                printf("\ncompression method: %d", hdr->d_compr);
                if (hdr->d_compr == 0) printf(" (stored)");
                else if (hdr->d_compr == 8) printf(" (deflated)");
                else printf(" (unknown)");
                printf("\ncrc32: %x\n", hdr->d_crc32);
                printf("compressed size: %d\n", hdr->d_csize);
                printf("uncompressed size: %d\n", hdr->d_usize);
                printf("offset of file in archive: %d\n", hdr->d_off);
                printf("filename: %s\n\n", hdr->d_name);
    
                if (hdr->d_reclen == 0) break;
                (char *)hdr += hdr->d_reclen;
                sleep(1);
            }
        }
    } printf ("\n}\n");
#endif
#if 1  
    { 	printf("{contents...\n");
        for (i = 0; i < 2; i++)
        {
            ZZIP_DIRENT* d;
    
            while((d=zzip_readdir(dir)))
            {
                printf(" name \"%s\", compr %d, size %d, ratio %2d\n", 
                    d->d_name, d->d_compr, d->st_size, 
                    100 - (d->d_csize|1)*100/(d->st_size|1));
            }
            printf(" %d. time ---------------\n", i + 1);
            zzip_rewinddir(dir);
        }
        printf("}...OK\n");
    }
#endif
  
    {   ZZIP_FILE * fp;
        char buf[17];
        const char * name = argv[1]? argv[1]: "readme";


        printf("Opening file `%s' in zip archive... ", name);    
        fp = zzip_file_open(dir, (char *)name, ZZIP_CASEINSENSITIVE);

        if (! fp)
          { printf("error %d: %s\n", zzip_error(dir), zzip_strerror_of(dir)); }
        else
        {
            printf("OK.\n");
            printf("Contents of the file:\n");

            while (0 < (i = zzip_file_read(fp, buf, 16)))
            {
                buf[i] = '\0';
                /*printf("\n*** read %d ***\n", i); fflush(stdout); */
                printf("%s", buf);
                /*write(1, buf, i);*/ /* Windows does not have write !!! */
            }
            if (i < 0) printf("error %d\n", zzip_error(dir));
        }
    }  
    
    return 0;
} 

/* 
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */
