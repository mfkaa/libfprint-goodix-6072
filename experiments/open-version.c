#include <stdio.h>
#include <libfprint/fprint.h>

int main(void)
{
    FpContext *ctx = fp_context_new();
    GPtrArray *devices = fp_context_get_devices(ctx);
    GError *error = NULL;
    int result = 0;

    if (devices->len == 0) {
        fprintf(stderr, "No fingerprint device found\n");
        g_object_unref(ctx);
        return 1;
    }

    FpDevice *dev = g_ptr_array_index(devices, 0);

    printf("Device: %s\n", fp_device_get_name(dev));
    printf("Opening device for version-only test...\n");

    if (!fp_device_open_sync(dev, NULL, &error)) {
        fprintf(stderr, "Open failed: %s\n", error->message);
        g_clear_error(&error);
        result = 1;
        goto out;
    }

    printf("Version-only initialization succeeded!\n");

    if (!fp_device_close_sync(dev, NULL, &error)) {
        fprintf(stderr, "Close failed: %s\n", error->message);
        g_clear_error(&error);
        result = 1;
    }

out:
    g_object_unref(ctx);
    return result;
}
