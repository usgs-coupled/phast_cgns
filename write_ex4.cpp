/*
    Creates simple 3-D structured grid and writes it to a
    CGNS file.
*/

#include <stdio.h>
#include <string.h>

#include "cgnslib.h"

#if CGNS_VERSION < 3100
# define cgsize_t int
#endif

int main()
{
    const cgsize_t ni = 16;
    const cgsize_t nj = 9;
    const cgsize_t nk = 5;

    // you can't do this in C
    double x[nk][nj][ni];
    double y[nk][nj][ni];
    double z[nk][nj][ni];

    cgsize_t isize[3][3];
    cgsize_t i, j, k;

    int index_file, icelldim, iphysdim, index_base;
    int index_zone, index_coord;
    char basename[33], zonename[33];

    double xmin = 0.0;
    double xmax = 90000.0;

    double ymin = 0.0;
    double ymax = 48000.0;

    double zmin = 0.0;
    double zmax = 400.0;

    /* create gridpoints for simple example: */
    for (k = 0; k < nk; ++k) {
        for (j = 0; j < nj; ++j) {
            for (i = 0; i < ni; ++i) {
                x[k][j][i] = xmin + i * (xmax - xmin) / (ni - 1);
                y[k][j][i] = ymin + j * (ymax - ymin) / (nj - 1);
                z[k][j][i] = zmin + k * (zmax - zmin) / (nk - 1);
            }
        }
    }

    /* WRITE X, Y, Z GRID POINTS TO CGNS FILE */
    
    /* open CGNS file for write */
    if (cg_open("ex4.cgns", CG_MODE_WRITE, &index_file)) {
        cg_error_exit();
    }
    
    /* create base (user can give any name) */
    strcpy(basename, "Base");

    icelldim = 3;
    iphysdim = 3;
    if (cg_base_write(index_file, basename, icelldim, iphysdim, &index_base)) {
        cg_error_exit();
    }
    
    /* define zone name (user can give any name) */
    strcpy(zonename, "Zone  1");
    
    /* vertex size */
    isize[0][0] = ni;
    isize[0][1] = nj;
    isize[0][2] = nk;
    
    /* cell size */
    isize[1][0] = isize[0][0] - 1;
    isize[1][1] = isize[0][1] - 1;
    isize[1][2] = isize[0][2] - 1;
    
    /* boundary vertex size (always zero for structured grids) */
    isize[2][0] = 0;
    isize[2][1] = 0;
    isize[2][2] = 0;
    
    /* create zone */
    if (cg_zone_write(index_file, index_base, zonename, *isize, CGNS_ENUMV(Structured), &index_zone)) {
        cg_error_exit();
    }

    /* write grid coordinates (user must use SIDS-standard names here) */
    if (cg_coord_write(index_file, index_base, index_zone, CGNS_ENUMV(RealDouble), "CoordinateX", x, &index_coord)) {
        cg_error_exit();
    }
    if (cg_coord_write(index_file, index_base, index_zone, CGNS_ENUMV(RealDouble), "CoordinateY", y, &index_coord)) {
        cg_error_exit();
    }
    if (cg_coord_write(index_file, index_base, index_zone, CGNS_ENUMV(RealDouble), "CoordinateZ", z, &index_coord)) {
        cg_error_exit();
    }
    
    /* close CGNS file */
    if (cg_close(index_file)) {
        cg_error_exit();
    }
    return 0;
}
