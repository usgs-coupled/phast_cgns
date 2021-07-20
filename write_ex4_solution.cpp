/*
    Creates simple 3-D structured grid and writes it to a
    CGNS file.
*/

#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>

#include "cgnslib.h"

#define ADD_UNITS

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

    double dens[nk][nj][ni];
    double pres[nk][nj][ni];

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

    int index_flow;
    int index_field;

    double time[3];

#ifdef ADD_UNITS
    float exponents[5];
#endif

    /* create gridpoints for simple example: */
    for (k = 0; k < nk; ++k) {
        for (j = 0; j < nj; ++j) {
            for (i = 0; i < ni; ++i) {
                x[k][j][i] = xmin + i * (xmax - xmin) / (ni - 1);
                y[k][j][i] = ymin + j * (ymax - ymin) / (nj - 1);
                z[k][j][i] = zmin + k * (zmax - zmin) / (nk - 1);
                dens[k][j][i] = z[k][j][i];
                pres[k][j][i] = y[k][j][i];
            }
        }
    }

    /* WRITE X, Y, Z GRID POINTS TO CGNS FILE */
    
    /* open CGNS file for write */
    if (cg_open("ex4_sol.cgns", CG_MODE_WRITE, &index_file)) {
        cg_error_exit();
    }
    
    /* create base (user can give any name) */
    strcpy(basename, "Base");

    icelldim = 3;
    iphysdim = 3;
    if (cg_base_write(index_file, basename, icelldim, iphysdim, &index_base)) {
        cg_error_exit();
    }

#ifdef ADD_UNITS
    // put DataClass and DimensionalUnits under Base
    cg_goto(index_file, index_base, "end");
    if (cg_dataclass_write(CGNS_ENUMV(Dimensional))) {
        cg_error_exit();
    }
    if (cg_units_write(CGNS_ENUMV(Kilogram), CGNS_ENUMV(Meter), CGNS_ENUMV(Second), CGNS_ENUMV(Kelvin), CGNS_ENUMV(Degree))) {
        cg_error_exit();
    }
#endif
    
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

    time[0] = 0.0;
    time[1] = 10.0;
    time[2] = 20.0;

    // define 3 different solution names (user can give any names)
    // solname
    // solname[0] = "FlowSolution1"
    // solname[1] = "FlowSolution2"
    // solname[2] = "FlowSolution3"

    cgsize_t dimvec;
    
    /* create zone */
    if (cg_zone_write(index_file, index_base, zonename, *isize, CGNS_ENUMV(Structured), &index_zone)) {
        cg_error_exit();
    }

#ifdef ADD_UNITS
    exponents[0]=0.;
    exponents[1]=1.;
    exponents[2]=0.;
    exponents[3]=0.;
    exponents[4]=0.;
#endif

    /* write grid coordinates (user must use SIDS-standard names here) */
    if (cg_coord_write(index_file, index_base, index_zone, CGNS_ENUMV(RealDouble), "CoordinateX", x, &index_coord)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "GridCoordinates_t", 1, "DataArray_t", index_coord, "end")) {
        cg_error_exit();
    }
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif

    if (cg_coord_write(index_file, index_base, index_zone, CGNS_ENUMV(RealDouble), "CoordinateY", y, &index_coord)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "GridCoordinates_t", 1, "DataArray_t", index_coord, "end")) {
        cg_error_exit();
    }
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif

    if (cg_coord_write(index_file, index_base, index_zone, CGNS_ENUMV(RealDouble), "CoordinateZ", z, &index_coord)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "GridCoordinates_t", 1, "DataArray_t", index_coord, "end")) {
        cg_error_exit();
    }
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif

    // create flow solution node
    if (cg_sol_write(index_file, index_base, index_zone, "FlowSolution1", CGNS_ENUMV(Vertex), &index_flow)) {
        cg_error_exit();
    }

    // write flow solution (user must use SIDS-standard names here)
    if (cg_field_write(index_file, index_base, index_zone, index_flow, CGNS_ENUMV(RealDouble), "Density", dens, &index_field)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "FlowSolution_t", 1, "DataArray_t", index_field, "end")) {
        cg_error_exit();
    }
    exponents[0] = 1.;
    exponents[1] = -3.;
    exponents[2] = 0.;
    exponents[3] = 0.;
    exponents[4] = 0.;
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif
    if (cg_field_write(index_file, index_base, index_zone, index_flow, CGNS_ENUMV(RealDouble), "Pressure", pres, &index_field)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "FlowSolution_t", 1, "DataArray_t", index_field, "end")) {
        cg_error_exit();
    }
    exponents[0] = 1.;
    exponents[1] = -1.;
    exponents[2] = -2.;
    exponents[3] = 0.;
    exponents[4] = 0.;
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif

    // create flow solution node
    if (cg_sol_write(index_file, index_base, index_zone, "FlowSolution2", CGNS_ENUMV(Vertex), &index_flow)) {
        cg_error_exit();
    }

    // write flow solution (user must use SIDS-standard names here)
    if (cg_field_write(index_file, index_base, index_zone, index_flow, CGNS_ENUMV(RealDouble), "Density", dens, &index_field)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "FlowSolution_t", 2, "DataArray_t", index_field, "end")) {
        cg_error_exit();
    }
    exponents[0] = 1.;
    exponents[1] = -3.;
    exponents[2] = 0.;
    exponents[3] = 0.;
    exponents[4] = 0.;
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif

    if (cg_field_write(index_file, index_base, index_zone, index_flow, CGNS_ENUMV(RealDouble), "Pressure", pres, &index_field)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "FlowSolution_t", 2, "DataArray_t", index_field, "end")) {
        cg_error_exit();
    }
    exponents[0] = 1.;
    exponents[1] = -1.;
    exponents[2] = -2.;
    exponents[3] = 0.;
    exponents[4] = 0.;
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif


    // create flow solution node
    if (cg_sol_write(index_file, index_base, index_zone, "FlowSolution3", CGNS_ENUMV(Vertex), &index_flow)) {
        cg_error_exit();
    }

    // write flow solution (user must use SIDS-standard names here)
    if (cg_field_write(index_file, index_base, index_zone, index_flow, CGNS_ENUMV(RealDouble), "Density", dens, &index_field)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "FlowSolution_t", 3, "DataArray_t", index_field, "end")) {
        cg_error_exit();
    }
    exponents[0] = 1.;
    exponents[1] = -3.;
    exponents[2] = 0.;
    exponents[3] = 0.;
    exponents[4] = 0.;
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif
    if (cg_field_write(index_file, index_base, index_zone, index_flow, CGNS_ENUMV(RealDouble), "Pressure", pres, &index_field)) {
        cg_error_exit();
    }
#ifdef ADD_UNITS
    if (cg_goto(index_file, index_base, "Zone_t", 1, "FlowSolution_t", 3, "DataArray_t", index_field, "end")) {
        cg_error_exit();
    }
    exponents[0] = 1.;
    exponents[1] = -1.;
    exponents[2] = -2.;
    exponents[3] = 0.;
    exponents[4] = 0.;
    if (cg_exponents_write(CGNS_ENUMV(RealSingle), exponents)) {
        cg_error_exit();
    }
#endif

    // create BaseIterativeData
    int nsteps=3;
    if (cg_biter_write(index_file, index_base, "TimeIterValues", nsteps)) {
        cg_error_exit();
    }

    // go to BaseIterativeData level and write time values
    dimvec = 3;
    if (cg_goto(index_file, index_base, "BaseIterativeData_t", 1, "end")) {
        cg_error_exit();
    }
    if (cg_array_write("TimeValues", CGNS_ENUMV(RealDouble), 1, &dimvec, time)) {
        cg_error_exit();
    }

    // create ZoneIterativeData
    if (cg_ziter_write(index_file, index_base, index_zone, "ZoneIterativeData")) {
        cg_error_exit();
    }

    // go to ZoneIterativeData level and give info telling which
    // flow solution corresponds with which time (solname(1) corresponds
    // with time(1), solname(2) with time(2), and solname(3) with time(3))
    if (cg_goto(index_file, index_base, "Zone_t", index_zone, "ZoneIterativeData_t", 1, "end")) {
        cg_error_exit();
    }
    cgsize_t idata[2];
    idata[0] = 32;
    idata[1] = 3;
    std::vector<char> buffer;
    buffer.assign((idata[0] * idata[1]), ' ');
    memcpy(buffer.data(), "FlowSolution1", 13);
    memcpy(buffer.data() + idata[0], "FlowSolution2", 13);
    memcpy(buffer.data() + idata[0]*2, "FlowSolution3", 13);
    if (cg_array_write("FlowSolutionPointers", CGNS_ENUMV(Character), 2, idata, buffer.data())) {
        cg_error_exit();
    }

    /* close CGNS file */
    if (cg_close(index_file)) {
        cg_error_exit();
    }

    printf("Ok\n");
    return 0;
}
