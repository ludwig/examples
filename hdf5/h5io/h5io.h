#ifndef __H5IO_H__
#define __H5IO_H__

#include "hdf5.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
 * File and group interface                                                 *
 ****************************************************************************/
hid_t h5io_file_create(const char *filename, const char *mode);
hid_t h5io_file_open(const char *filename, const char *mode);

hid_t h5io_group_create(hid_t loc_id, const char *name);
hid_t h5io_group_open(hid_t loc_id, const char *name);

hid_t h5io_group_touch(hid_t loc_id, const char *name);
hid_t h5io_group_touch_path(hid_t loc_id, const char *path);



/****************************************************************************
 * Attribute interface                                                      *
 ****************************************************************************/

herr_t h5io_attr_find(hid_t loc_id, const char *attr_name);
herr_t h5io_attr_set(hid_t obj_id, const char *attr_name, hid_t type_id, const void *data);
herr_t h5io_attr_get(hid_t obj_id, const char *attr_name, hid_t type_id, void *data);
herr_t h5io_attr_get_dims(hid_t obj_id, const char *attr_name, int *rank, hsize_t *dims);

herr_t h5io_attr_set_str(hid_t obj_id, const char *attr_name, const char *attr_data);
herr_t h5io_attr_get_str(hid_t obj_id, const char *attr_name, char **data);

herr_t h5io_attr_set_int(hid_t obj_id, const char *attr_name, int n);
herr_t h5io_attr_set_long(hid_t obj_id, const char *attr_name, long n);
herr_t h5io_attr_set_llong(hid_t obj_id, const char *attr_name, long long n);
herr_t h5io_attr_set_float(hid_t obj_id, const char *attr_name, float x);
herr_t h5io_attr_set_double(hid_t obj_id, const char *attr_name, double x);

herr_t h5io_attr_set_array(hid_t obj_id, const char *attr_name,
                           size_t rank, hsize_t *dims, hid_t type_id, const void *data);

herr_t h5io_attr_set_array1(hid_t obj_id, const char *attr_name,
                            hsize_t dim, hid_t type_id, const void *data);

herr_t h5io_attr_set_array1_of_ints(hid_t obj_id, const char *attr_name,
                                    hsize_t dim, const int *data);

herr_t h5io_attr_set_array1_of_floats(hid_t obj_id, const char *attr_name,
                                      hsize_t dim, const float *data);

herr_t h5io_attr_set_array1_of_doubles(hid_t obj_id, const char *attr_name,
                                       hsize_t dim, const double *data);

/****************************************************************************
 * Dataset interface                                                        *
 ****************************************************************************/

hid_t h5io_dset_create(hid_t loc_id, const char *name, const char *title,
                       hid_t type_id, int rank, int *shape);

hid_t h5io_dset_open(hid_t loc_id, const char *name,
                     hid_t *type_id, int *rank, int *shape, int *npoints);


int h5io_dset_read(hid_t loc_id, const char *name,
                   hid_t type_id, int rank, int *shape, void **data);

int h5io_dset_read1(hid_t loc_id, const char *name,
                    hid_t type_id, void **data, int *n);

int h5io_dset_read2(hid_t loc_id, const char *name,
                    hid_t type_id, void **data, int *m, int *n);

int h5io_dset_read3(hid_t loc_id, const char *name,
                    hid_t type_id, void **data, int *l, int *m, int *n);


int h5io_dset_write(hid_t loc_id, const char *name, const char *title,
                    hid_t type_id, int rank, int *shape, void *data);

int h5io_dset_write1(hid_t loc_id, const char *name, const char *title,
                     hid_t type_id, void *data, int n);

int h5io_dset_write2(hid_t loc_id, const char *name, const char *title,
                     hid_t type_id, void *data, int m, int n);

int h5io_dset_write3(hid_t loc_id, const char *name, const char *title,
                     hid_t type_id, void *data, int l, int m, int n);


int h5io_dset_slice_read3(hid_t loc_id,
                          const char *name,
                          hid_t type_id,
                          int rank, hsize_t *dims, void *data,
                          int start, int end);

int h5io_dset_slice_write3(hid_t loc_id,
                           const char *name,
                           const char *title,
                           hid_t type_id,
                           int start, int end);

/****************************************************************************
 * Array interface                                                          *
 ****************************************************************************/

typedef struct {
    int rank;
    hsize_t *shape;
    hid_t type_id;
    void *data;
} h5array_t;

int h5io_array_init(h5array_t *array, hid_t type_id, int rank, int *shape, void *data);
int h5io_array_free_dims(h5array_t *array);
int h5io_array_free_data(h5array_t *array);
int h5io_array_free(h5array_t *array);

int h5io_array_npoints(h5array_t *array);
int h5io_array_dim(h5array_t *array, int i);
void h5io_array_dims(h5array_t *array, int *rank, int **shape);
void h5io_array_dims1(h5array_t *array, int *n);
void h5io_array_dims2(h5array_t *array, int *m, int *n);
void h5io_array_dims3(h5array_t *array, int *l, int *m, int *n);

hid_t h5io_array_open(h5array_t *array, hid_t loc_id, const char *name);
hid_t h5io_array_create(h5array_t *array, hid_t loc_id, const char *name, const char *title);

int h5io_array_read(h5array_t *array, hid_t loc_id, const char *name);
int h5io_array_write(h5array_t *array, hid_t loc_id, const char *name);

int h5io_array_slice_read(h5array_t *array, hid_t dataset_id, int start, int end);
int h5io_array_slice_write(h5array_t *array, hid_t dataset_id, int start, int end);


#ifdef __cplusplus
}
#endif

#endif /* __H5IO_H__ */
