
#include <stdio.h>
#include <libfprint/fprint.h>

int main(void)
{
    FpContext *ctx = fp_context_new();
    GPtrArray *devices = fp_context_get_devices(ctx);
    GPtrArray *prints = NULL;
    GError *error = NULL;
    FpDevice *dev;
    int result = 1;
    gboolean opened = FALSE;

    if (devices->len != 1) {
        fprintf(stderr, "Expected one fingerprint device, found %u\n",
                devices->len);
        goto cleanup;
    }

    dev = g_ptr_array_index(devices, 0);

    if (g_strcmp0(fp_device_get_driver(dev), "goodixmoc") != 0) {
        fprintf(stderr, "Unexpected fingerprint driver\n");
        goto cleanup;
    }

    if (!fp_device_open_sync(dev, NULL, &error)) {
        fprintf(stderr, "Open failed: %s\n", error->message);
        goto cleanup;
    }

    opened = TRUE;

    prints = fp_device_list_prints_sync(dev, NULL, &error);

    if (!prints) {
        fprintf(stderr, "List failed: %s\n",
                error ? error->message : "Unknown error");
        goto cleanup;
    }

    printf("Stored fingerprints: %u\n", prints->len);
    result = 0;

cleanup:
    if (opened) {
        GError *close_error = NULL;

        if (!fp_device_close_sync(dev, NULL, &close_error)) {
            fprintf(stderr, "Close failed: %s\n",
                    close_error->message);
            result = 1;
        }

        g_clear_error(&close_error);
    }

    if (prints)
        g_ptr_array_unref(prints);

    g_clear_error(&error);
    g_object_unref(ctx);

    return result;
}

