
#include <glib.h>

#include "fpi-byte-reader.h"
#include "goodix_proto.h"

static void
test_ack_success (void)
{
  guint8 body[] = { GX_SUCCESS, MOC_CMD0_GET_VERSION };
  FpiByteReader reader;
  gxfp_cmd_response_t response = { 0 };

  fpi_byte_reader_init (&reader, body, sizeof (body));

  g_assert_cmpint (
    gx_proto_parse_body (
      MAKE_CMD_EX (RESPONSE_PACKAGE_CMD, MOC_CMD1_DEFAULT),
      &reader, &response),
    ==, 0);

  g_assert_cmphex (response.result, ==, GX_SUCCESS);
  g_assert_cmphex (response.parse_msg.ack_cmd,
                   ==, MOC_CMD0_GET_VERSION);
}

static void
test_ack_error (void)
{
  guint8 body[] = { GX_FAILED, MOC_CMD0_UPDATE_CONFIG };
  FpiByteReader reader;
  gxfp_cmd_response_t response = { 0 };

  fpi_byte_reader_init (&reader, body, sizeof (body));

  g_assert_cmpint (
    gx_proto_parse_body (
      MAKE_CMD_EX (RESPONSE_PACKAGE_CMD, MOC_CMD1_DEFAULT),
      &reader, &response),
    ==, 0);

  g_assert_cmphex (response.result, ==, GX_FAILED);
  g_assert_cmphex (response.parse_msg.ack_cmd,
                   ==, MOC_CMD0_UPDATE_CONFIG);
}

static void
test_config_data (void)
{
  guint8 body[] = { GX_SUCCESS, 0x00, 0x09 };
  FpiByteReader reader;
  gxfp_cmd_response_t response = { 0 };

  fpi_byte_reader_init (&reader, body, sizeof (body));

  g_assert_cmpint (
    gx_proto_parse_body (
      MAKE_CMD_EX (MOC_CMD0_UPDATE_CONFIG,
                   MOC_CMD1_NWRITE_CFG_TO_FLASH),
      &reader, &response),
    ==, 0);

  g_assert_cmphex (response.finger_config.status,
                   ==, GX_SUCCESS);
  g_assert_cmpuint (response.finger_config.max_stored_prints,
                    ==, 9);
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/goodixmoc/ack-success", test_ack_success);
  g_test_add_func ("/goodixmoc/ack-error", test_ack_error);
  g_test_add_func ("/goodixmoc/config-data", test_config_data);

  return g_test_run ();
}

