void kernel copy_array(global const float *source, global float *target) {
    target[get_global_id(0)] = source[get_global_id(0)];
}
