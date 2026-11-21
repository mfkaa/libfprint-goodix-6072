#include <stdio.h>
#include <libfprint/fprint.h>

int main(void)
{
    FpContext *ctx = fp_context_new();
    GPtrArray *devices = fp_context_get_devices(ctx);

    printf("Discovered %u fingerprint device(s)\n", devices->len);

    for (guint i = 0; i < devices->len; i++) {
        FpDevice *dev = g_ptr_array_index(devices, i);

        printf("Device: %s\n", fp_device_get_name(dev));
        printf("Driver: %s\n", fp_device_get_driver(dev));
    }

    g_object_unref(ctx);
    return 0;
}
