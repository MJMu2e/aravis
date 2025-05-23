/* Aravis - Digital camera library
 *
 * Copyright © 2009-2025 Emmanuel Pacaud <emmanuel.pacaud@free.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Emmanuel Pacaud <emmanuel.pacaud@free.fr>
 */

#include <glib/gprintf.h>
#include <arv.h>
#include <stdlib.h>
#include <stdio.h>
#include <arvdebugprivate.h>
#include <arvmiscprivate.h>
#include <arvgcprivate.h>
#include <arvbufferprivate.h>
#include <math.h>

#ifdef _MSC_VER
#define STDOUT_FILENO _fileno(stdout)
#else
#include <unistd.h>
#endif

typedef enum {
        ARV_TEST_STATUS_SUCCESS,
        ARV_TEST_STATUS_FAILURE,
        ARV_TEST_STATUS_IGNORED
} ArvTestStatus;

typedef struct {
        char *test_name;
        char *camera_name;
        ArvTestStatus status;
        char *comment;
} ArvTestResult;

#define ARV_TYPE_TEST_RESULT (arv_test_result_get_type())
GType arv_test_result_get_type(void);

static ArvTestResult *
arv_test_result_new (const char *test_name, const char *camera_name, ArvTestStatus status, const char *comment)
{
        ArvTestResult *result = g_new0 (ArvTestResult, 1);

        result->test_name = g_strdup (test_name);
        result->camera_name = g_strdup (camera_name);
        result->status = status;
        result->comment = g_strdup (comment);

        return result;
}

static ArvTestResult *
arv_test_result_copy (ArvTestResult *result)
{
        return arv_test_result_new (result->test_name, result->camera_name, result->status, result->comment);
}

static void
arv_test_result_free (ArvTestResult *result)
{
        if (result != NULL) {
                g_free (result->test_name);
                g_free (result->camera_name);
                g_free (result->comment);
                g_free (result);
        }
}

G_DEFINE_BOXED_TYPE (ArvTestResult, arv_test_result, arv_test_result_copy, arv_test_result_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (ArvTestResult, arv_test_result_free)

typedef struct {
        char *id;
        ArvCamera *camera;
        char *vendor_model;
        GSList *results;
        gboolean cache_check;
} ArvTestCamera;

#define ARV_TYPE_TEST_CAMERA (arv_test_camera_get_type())
GType arv_test_camera_get_type(void);

static ArvTestCamera *
arv_test_camera_new (const char *camera_id, gboolean cache_check)
{
        ArvTestCamera *test_camera;
        ArvCamera *camera = arv_camera_new (camera_id, NULL);

        if (!ARV_IS_CAMERA (camera))
                return NULL;

        test_camera = g_new0 (ArvTestCamera, 1);
        test_camera->id = g_strdup (camera_id);
        test_camera->camera = camera;
        test_camera->vendor_model = g_strdup_printf ("%s:%s",
                                                     arv_camera_get_vendor_name (test_camera->camera, NULL),
                                                     arv_camera_get_model_name (test_camera->camera, NULL));
        test_camera->cache_check = cache_check;

        if (cache_check)
                arv_camera_set_register_cache_policy (test_camera->camera, ARV_REGISTER_CACHE_POLICY_DEBUG);

        return test_camera;
}

static ArvTestCamera *
arv_test_camera_copy (ArvTestCamera *self)
{
        return arv_test_camera_new (self->id, self->cache_check);
}

static void
arv_test_camera_free (ArvTestCamera *camera)
{
        if (camera != NULL) {
                if (camera->results != NULL)
                        g_slist_free_full (camera->results, (GDestroyNotify) arv_test_result_free);
                g_clear_pointer (&camera->id, g_free);
                g_clear_object (&camera->camera);
                g_clear_pointer (&camera->vendor_model, g_free);
                g_free (camera);
        }
}

static gboolean
stdout_has_color_support (void)
{
#if GLIB_CHECK_VERSION(2,50,0)
	static int has_color_support = -1;

	if (has_color_support >= 0)
		return has_color_support > 0;

	has_color_support = g_log_writer_supports_color (STDOUT_FILENO) ? 1 : 0;

	return has_color_support;
#else
	return FALSE;
#endif
}

static void
arv_test_camera_add_result (ArvTestCamera *test_camera,
                            const char *test_name, const char *step_name,
                            ArvTestStatus status, const char *comment)
{
        char *title;
        const char *status_str;

        title = g_strdup_printf ("%s:%s", test_name, step_name);

        if (stdout_has_color_support ())
                switch (status) {
                        case ARV_TEST_STATUS_SUCCESS: status_str = "\033[1;32mSUCCESS\033[0m"; break;
                        case ARV_TEST_STATUS_FAILURE: status_str = "\033[1;31mFAILURE\033[0m"; break;
                        case ARV_TEST_STATUS_IGNORED: status_str = "\033[1;34mIGNORED\033[0m"; break;
                        default: status_str = "";
                }
        else
                switch (status) {
                        case ARV_TEST_STATUS_SUCCESS: status_str = "SUCCESS"; break;
                        case ARV_TEST_STATUS_FAILURE: status_str = "FAILURE"; break;
                        case ARV_TEST_STATUS_IGNORED: status_str = "IGNORED"; break;
                        default: status_str = "";
                }

        g_fprintf (stdout, "%-35s %s %s\n", title, status_str, comment != NULL ? comment : "");

        test_camera->results = g_slist_append (test_camera->results,
                                               arv_test_result_new (title, test_camera->vendor_model,
                                                                    status, comment));

        g_clear_pointer (&title, g_free);
}

G_DEFINE_BOXED_TYPE (ArvTestCamera, arv_test_camera, arv_test_camera_copy, arv_test_camera_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (ArvTestCamera, arv_test_camera_free)

#define ARV_TYPE_TEST arv_test_get_type()
G_DECLARE_FINAL_TYPE (ArvTest, arv_test, ARV, TEST, GObject)

struct _ArvTest {
        GObject parent;

        GKeyFile *key_file;

        ArvXmlSchema *schema_1_1;
        ArvXmlSchema *schema_1_0;
};

G_DEFINE_TYPE (ArvTest, arv_test, G_TYPE_OBJECT)

static void
arv_test_finalize (GObject *gobject)
{
        ArvTest *self = ARV_TEST (gobject);

	g_clear_object (&self->schema_1_1);
	g_clear_object (&self->schema_1_0);

        g_clear_pointer (&self->key_file, g_key_file_unref);

        G_OBJECT_CLASS (arv_test_parent_class)->finalize (gobject);
}

static void
arv_test_class_init (ArvTestClass *test_class)
{
        GObjectClass *object_class = G_OBJECT_CLASS (test_class);

        object_class->finalize = arv_test_finalize;
}

static void
arv_test_init (ArvTest *self)
{
        GBytes *bytes = NULL;

        bytes = g_resources_lookup_data("/org/aravis/GenApiSchema_Version_1_0.xsd", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
        self->schema_1_0 = arv_xml_schema_new_from_memory (g_bytes_get_data (bytes, NULL), g_bytes_get_size (bytes));
        g_clear_pointer (&bytes, g_bytes_unref);

        bytes = g_resources_lookup_data("/org/aravis/GenApiSchema_Version_1_0.xsd", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
        self->schema_1_1 = arv_xml_schema_new_from_memory (g_bytes_get_data (bytes, NULL), g_bytes_get_size (bytes));
        g_clear_pointer (&bytes, g_bytes_unref);

        bytes = g_resources_lookup_data ("/org/aravis/arv-test.cfg", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
        self->key_file = g_key_file_new ();
        g_key_file_load_from_data (self->key_file, g_bytes_get_data (bytes, NULL), g_bytes_get_size (bytes),
                                   G_KEY_FILE_KEEP_COMMENTS, NULL);
        g_clear_pointer (&bytes, g_bytes_unref);
}

static ArvTest *
arv_test_new (void)
{
        return g_object_new (ARV_TYPE_TEST, NULL);
}

static double
arv_test_camera_get_key_file_double (ArvTestCamera *test_camera, ArvTest *test, const char *key, double default_value)
{
        GError *error = NULL;
        double value;

        g_return_val_if_fail (test_camera != NULL, 0);
        g_return_val_if_fail (ARV_IS_TEST (test), 0);

        value = g_key_file_get_double (test->key_file, test_camera->vendor_model, key, &error);
        if (error != NULL) {
                g_clear_error (&error);
                return default_value;
        }

        return value;
}

static gint64
arv_test_camera_get_key_file_int64 (ArvTestCamera *test_camera, ArvTest *test, const char *key, gint64 default_value)
{
        GError *error = NULL;
        gint64 value;

        g_return_val_if_fail (test_camera != NULL, 0);
        g_return_val_if_fail (ARV_IS_TEST (test), 0);

        value = g_key_file_get_int64 (test->key_file, test_camera->vendor_model, key, &error);
        if (error != NULL) {
                g_clear_error (&error);
                return default_value;
        }

        return value;
}

static gboolean
arv_test_camera_get_key_file_boolean (ArvTestCamera *test_camera, ArvTest *test, const char *key, gboolean default_value)
{
        GError* error = NULL;
        gboolean value;

        g_return_val_if_fail (test_camera != NULL, 0);
        g_return_val_if_fail (ARV_IS_TEST (test), 0);

        value = g_key_file_get_boolean (test->key_file, test_camera->vendor_model, key, &error);
        if (error != NULL) {
                g_clear_error (&error);
                return default_value;
        }

        return value;
}

static char *
arv_test_camera_get_key_file_string (ArvTestCamera *test_camera, ArvTest *test, const char *key,
                                     const char *default_value)
{
        GError *error = NULL;
        char *value;

        g_return_val_if_fail (test_camera != NULL, NULL);
        g_return_val_if_fail (ARV_IS_TEST (test), NULL);

        value = g_key_file_get_string (test->key_file, test_camera->vendor_model, key, &error);
        if (error != NULL) {
                g_clear_error (&error);
                g_free (value);
                return g_strdup (default_value);
        }

        return value;
}

static char *
arv_test_camera_get_key_file_comment (ArvTestCamera *test_camera, ArvTest *test, const char *key)
{
        g_return_val_if_fail (test_camera != NULL, NULL);
        g_return_val_if_fail (ARV_IS_TEST (test), NULL);

        return g_key_file_get_comment (test->key_file, test_camera->vendor_model, key, NULL);
}

static gint *
arv_test_camera_get_key_file_integer_list (ArvTestCamera *test_camera, ArvTest *test, const char *key, gsize *size)
{
        if (size != NULL)
                *size = 0;

        g_return_val_if_fail (test_camera != NULL, NULL);
        g_return_val_if_fail (ARV_IS_TEST (test), NULL);

        return g_key_file_get_integer_list (test->key_file, test_camera->vendor_model, key, size, NULL);
}

static guint64
arv_test_camera_get_n_register_cache_errors (ArvTestCamera *test_camera)
{
        ArvGc *genicam;

        genicam = arv_device_get_genicam (arv_camera_get_device (test_camera->camera));

        return arv_gc_register_cache_error_add (genicam, 0);
}

static void
arv_test_genicam (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
	ArvDevice *device;
        ArvTestStatus status;
        char *version = NULL;
        char *comment = NULL;
	const char *genicam;
	size_t size;

        g_return_if_fail (ARV_IS_TEST (test));

        version = arv_test_camera_get_key_file_string (test_camera, test, "Schema", "Invalid");

	device = arv_camera_get_device (test_camera->camera);
	genicam = arv_device_get_genicam_xml (device, &size);

        arv_test_camera_add_result (test_camera, test_name, "Load",
                                    genicam != NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    genicam != NULL ? "" : "Failed to retrieve Genicam data");

	if (genicam == NULL) {
                g_free (version);
                return;
        }

        status = ARV_TEST_STATUS_IGNORED;

        if (g_strcmp0 (version, "1.1") == 0) {
                if (arv_xml_schema_validate (test->schema_1_1, genicam, size, NULL, NULL, NULL)) {
                        status = ARV_TEST_STATUS_SUCCESS;
                        comment = g_strdup_printf ("%s", version);
                } else {
                        status = ARV_TEST_STATUS_FAILURE;
                }
        } else if (g_strcmp0 (version, "1.0") == 0) {
                if (arv_xml_schema_validate (test->schema_1_0, genicam, size, NULL, NULL, NULL)) {
                        status = ARV_TEST_STATUS_SUCCESS;
                        comment = g_strdup_printf ("%s", version);
                } else {
                        status = ARV_TEST_STATUS_FAILURE;
                }
        } else if (version != NULL && g_ascii_strcasecmp (version, "Invalid") == 0) {
                status = ARV_TEST_STATUS_SUCCESS;
                        comment = g_strdup ("Schema validation disabled");
        }

        arv_test_camera_add_result (test_camera, test_name, "Schema", status, comment);

        g_free (version);
        g_free (comment);
}

static void
arv_test_device_properties (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        GError *error = NULL;
        gint *sensor_size = NULL;
        char *comment = NULL;
        ArvTestStatus status;
        gint sensor_width, sensor_height;
        gsize size = 0;
        gboolean available;

        g_return_if_fail (ARV_IS_TEST (test));

        sensor_size = arv_test_camera_get_key_file_integer_list (test_camera, test, "SensorSize", &size);

	arv_camera_get_sensor_size (test_camera->camera, &sensor_width, &sensor_height, &error);
        arv_test_camera_add_result (test_camera, test_name, "SensorSizeReadout",
                                    error == NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    error != NULL ? error->message : NULL);

        if (error == NULL && size == 2) {
                if (sensor_size[0] != sensor_width ||
                    sensor_size[1] != sensor_height) {
                        status = ARV_TEST_STATUS_FAILURE;
                        comment = g_strdup_printf ("Found %dx%d instead of %dx%d",
                                                   sensor_width, sensor_height,
                                                   sensor_size[0], sensor_size[1]);
                } else {
                        status = ARV_TEST_STATUS_SUCCESS;
                        comment = g_strdup_printf ("%dx%d", sensor_size[0], sensor_size[1]);
                }
        }  else {
                status = ARV_TEST_STATUS_IGNORED;
        }

        arv_test_camera_add_result (test_camera, test_name, "SensorSizeCheck", status, comment);

        g_clear_error (&error);
        available = arv_camera_is_gain_available(test_camera->camera, &error);
        arv_test_camera_add_result (test_camera, test_name, "GainAvailable",
                                    error == NULL &&
                                    available == arv_test_camera_get_key_file_boolean(test_camera, test,
                                                                                      "GainAvailable",
                                                                                      TRUE) ?
                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    error != NULL ? error->message : NULL);

        if (available) {
                g_clear_error (&error);
                arv_camera_get_gain (test_camera->camera, &error);
                arv_test_camera_add_result (test_camera, test_name, "GainReadout",
                                            error == NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                            error != NULL ? error->message : NULL);
        }

        g_clear_error (&error);
        available = arv_camera_is_exposure_time_available (test_camera->camera, &error);
        arv_test_camera_add_result (test_camera, test_name, "ExposureTimeAvailable",
                                    error == NULL &&
                                    available == arv_test_camera_get_key_file_boolean(test_camera, test,
                                                                                      "ExposureTimeAvailable",
                                                                                      TRUE) ?
                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    error != NULL ? error->message : NULL);

        if (available) {
                g_clear_error (&error);
                arv_camera_get_exposure_time (test_camera->camera, &error);
                arv_test_camera_add_result (test_camera, test_name, "ExposureTimeReadout",
                                            error == NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                            error != NULL ? error->message : NULL);
        }

        g_clear_error (&error);
        g_free (sensor_size);
        g_free (comment);
}

static void
_single_acquisition (ArvTest *test, const char *test_name, ArvTestCamera *test_camera,
                     gboolean chunk_test, gboolean multipart_test)
{
        GError *error = NULL;
        ArvBuffer *buffer = NULL;
        char **chunk_list = NULL;
        char **chunk_selector_list = NULL;
        ArvChunkParser *parser = NULL;
        char *component = NULL;
        guint n_parts = 1;
        GString *message;

        g_return_if_fail (ARV_IS_TEST (test));

        g_assert (!(multipart_test & chunk_test));

        if (multipart_test) {
                char  *multiparts = arv_test_camera_get_key_file_string (test_camera, test, "Multipart", NULL);
                char **parts;
                gboolean multipart_supported;
                gboolean multipart_expected;
                guint i;

                parts = multiparts != NULL ? g_strsplit(multiparts, " ", -1) : NULL;
                n_parts = parts != NULL ? g_strv_length(parts) : 0;

                multipart_supported = arv_camera_gv_is_multipart_supported (test_camera->camera, &error);
                multipart_expected = n_parts > 1;

                if (!multipart_expected || (multipart_supported != multipart_expected)) {
                        arv_test_camera_add_result (test_camera, test_name, "NoSupport",
                                                    error == NULL && (multipart_expected == multipart_supported) ?
                                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                                    error != NULL ? error->message : NULL);
                        g_clear_error (&error);
                        return;
                }

                arv_camera_gv_set_multipart(test_camera->camera, TRUE, &error);
                arv_test_camera_add_result (test_camera, test_name, "Enable",
                                            error == NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                            error != NULL ? error->message : NULL);
                if (error != NULL) {
                        g_clear_error (&error);
                        return;
                }

                for (i = 0; i < n_parts && error == NULL; i++) {
                        arv_camera_select_and_enable_component(test_camera->camera, parts[i], i == 0, &error);
                }

                component = g_strdup (parts[0]);

                g_clear_pointer (&parts, g_strfreev);
                g_free (multiparts);
        }

        if (chunk_test) {
                char *chunks;
                char *chunk_selector;
                gboolean chunks_support = arv_test_camera_get_key_file_boolean (test_camera, test,
                                                                                "ChunksSupport", TRUE);
                n_parts = arv_test_camera_get_key_file_int64 (test_camera, test,
                                                              "ChunksNParts", 1);

                if (!arv_camera_are_chunks_available (test_camera->camera, &error)) {
                        arv_test_camera_add_result (test_camera, test_name, "NoSupport",
                                                    error == NULL && ! chunks_support ?
                                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                                    error != NULL ? error->message : NULL);
			g_clear_error (&error);
                        return;
                }

                chunks = arv_test_camera_get_key_file_string (test_camera, test, "ChunkList", "Width Height");
                chunk_list = g_strsplit_set (chunks, " ", -1);

                chunk_selector = arv_test_camera_get_key_file_string(test_camera, test,
                                                                     "ChunkSelector", "Ignore Ignore");
                chunk_selector_list = g_strsplit_set(chunk_selector, " ", -1);

                parser = arv_camera_create_chunk_parser (test_camera->camera);

                arv_camera_set_chunks (test_camera->camera, chunks, &error);

                g_free (chunks);
                g_free (chunk_selector);
        }

        message = g_string_new ("");

        if (error == NULL)
                buffer = arv_camera_acquisition (test_camera->camera, 1000000, &error);

        if (error == NULL && (!ARV_IS_BUFFER (buffer) || arv_buffer_get_status(buffer) != ARV_BUFFER_STATUS_SUCCESS)) {
                g_set_error (&error, ARV_DEVICE_ERROR, ARV_DEVICE_ERROR_TRANSFER_ERROR,
                             "Buffer transfer failure");
        }

        if (error == NULL && ARV_IS_BUFFER (buffer)) {
                if (arv_buffer_get_n_parts(buffer) != n_parts)
                        g_set_error (&error, ARV_DEVICE_ERROR, ARV_DEVICE_ERROR_TRANSFER_ERROR,
                                     "Invalid number of parts (found: %u - expected: %u)",
                                     arv_buffer_get_n_parts(buffer), n_parts);
        }

        if (chunk_test) {
                if (error == NULL) {
                        int n_chunks = g_strv_length (chunk_list);
                        int n_elements = g_strv_length (chunk_selector_list) - 1;
                        int i, j;

                        if (arv_buffer_has_chunks(buffer)) {
                                for (i = 0; i < n_chunks && error == NULL; i++) {
                                        char *chunk_name = g_strdup_printf ("Chunk%s", chunk_list[i]);
                                        for (j = 0; j < n_elements && error == NULL; j++) {
                                                if (g_strcmp0 (chunk_selector_list[0], "Ignore") != 0) {
                                                        arv_chunk_parser_set_string_feature_value
                                                                (parser,
                                                                 chunk_selector_list[0],
                                                                 chunk_selector_list[j+1],
                                                                 &error);
                                                }
                                                if (error == NULL) {
                                                        gint64 int_value;

                                                        int_value = arv_chunk_parser_get_integer_value (parser, buffer,
                                                                                            chunk_name, &error);
                                                        if (error != NULL) {
                                                                double float_value;

                                                                g_clear_error (&error);
                                                                float_value = arv_chunk_parser_get_float_value
                                                                        (parser, buffer, chunk_name, &error);
                                                                if (error == NULL) {
                                                                        g_string_append_printf
                                                                                (message,
                                                                                 "%s%g",
                                                                                 message->len > 0 ?  " ": "",
                                                                                 float_value);
                                                                }
                                                        } else {
                                                                g_string_append_printf (message, "%s%"
                                                                                        G_GINT64_MODIFIER "d",
                                                                                        message->len > 0 ? " ": "",
                                                                                        int_value);
                                                        }
                                                }
                                        }
                                        g_clear_pointer (&chunk_name, g_free);
                                }
                        } else {
                                g_set_error (&error, ARV_DEVICE_ERROR, ARV_DEVICE_ERROR_TRANSFER_ERROR,
                                             "No chunk found in buffer");
                        }

                        g_clear_object (&parser);
                }
                g_strfreev (chunk_list);
                g_strfreev (chunk_selector_list);
        }

        if (chunk_test)
                arv_camera_set_chunks (test_camera->camera, NULL, NULL);

        if (error == NULL && multipart_test) {
                arv_camera_gv_set_multipart (test_camera->camera, FALSE, &error);
                if (error == NULL) arv_camera_select_and_enable_component(test_camera->camera, component, TRUE, &error);

                g_free (component);
        }

        arv_test_camera_add_result (test_camera, test_name, "BufferCheck",
                                    ARV_IS_BUFFER (buffer) &&
                                    arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_SUCCESS &&
                                    error == NULL ?
                                    ARV_TEST_STATUS_SUCCESS :
                                    ARV_TEST_STATUS_FAILURE,
                                    error != NULL ? error->message : message->str);

        g_string_free (message, TRUE);
        g_clear_error (&error);
        g_clear_object (&buffer);
}

static void
arv_test_single_acquisition (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        _single_acquisition (test, test_name, test_camera, FALSE, FALSE);
}

static void
arv_test_multipart (ArvTest *test, const char  *test_name, ArvTestCamera *test_camera)
{
        if (!arv_camera_is_gv_device (test_camera->camera))
                return;

        _single_acquisition (test, test_name, test_camera, FALSE, TRUE);
}

static void
arv_test_chunks (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        _single_acquisition (test, test_name, test_camera, TRUE, FALSE);
}

typedef struct {
        unsigned int n_init;
        unsigned int n_start;
        unsigned int n_done;
        unsigned int n_success;
        unsigned int n_exit;
} _StreamCallbackData;

static void
_stream_calback (gpointer data, ArvStreamCallbackType type, ArvBuffer *buffer)
{
        _StreamCallbackData *callback_data = data;

        switch (type) {
                case ARV_STREAM_CALLBACK_TYPE_INIT:
                        callback_data->n_init++;
                        break;
                case ARV_STREAM_CALLBACK_TYPE_START_BUFFER:
                        callback_data->n_start++;
                        break;
                case ARV_STREAM_CALLBACK_TYPE_BUFFER_DONE:
                        callback_data->n_done++;
                        if (buffer->priv->status == ARV_BUFFER_STATUS_SUCCESS)
                                callback_data->n_success++;
                        break;
                case ARV_STREAM_CALLBACK_TYPE_EXIT:
                        callback_data->n_exit++;
                        break;
                default:
                        break;
        }
}

static void
_multiple_acquisition (ArvTest *test, const char *test_name, ArvTestCamera *test_camera,
                       double frame_rate, gboolean use_system_timestamp)
{
        GError *error = NULL;
        char *message = NULL;
        ArvStream *stream;
        _StreamCallbackData callback_data;
        unsigned int i;
        size_t payload_size;
        gboolean success = TRUE;
        gint64 start_time = -1;
        gint64 end_time = -1;
        gboolean frame_rate_success;
        gboolean callback_init_success;
        gboolean callback_buffer_success;
        gboolean callback_exit_success;
        guint n_completed_buffers = 0;
        guint n_expected_buffers = 10;

        arv_camera_set_acquisition_mode (test_camera->camera, ARV_ACQUISITION_MODE_CONTINUOUS, &error);
        if (error == NULL)
                arv_camera_set_frame_rate (test_camera->camera, frame_rate, &error);

        if (error != NULL) {
                arv_test_camera_add_result (test_camera, test_name, "BufferCheck",
                                            ARV_TEST_STATUS_FAILURE, error->message);
                g_clear_error (&error);
                return;
        }

        callback_data.n_init = 0;
        callback_data.n_start = 0;
        callback_data.n_done = 0;
        callback_data.n_success = 0;
        callback_data.n_exit = 0;
        stream = arv_camera_create_stream (test_camera->camera, _stream_calback, &callback_data, NULL, &error);
        if (error == NULL)
                payload_size = arv_camera_get_payload (test_camera->camera, &error);
        if (error == NULL) {
                for (i = 0 ; i < 2; i++)
                        arv_stream_push_buffer (stream, arv_buffer_new (payload_size, FALSE));
        }
        if (error == NULL)
                arv_camera_start_acquisition (test_camera->camera, &error);
        if (error == NULL) {
                for (i = 0 ; i < n_expected_buffers; i++) {
                        ArvBuffer *buffer;

                        buffer = arv_stream_timeout_pop_buffer (stream, i == 0 ? 5000000 : 500000);
                        if (buffer == NULL)
                                success = FALSE;
                        else {
                                if (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_SUCCESS) {
                                        n_completed_buffers++;
                                        if (start_time < 0)
                                                start_time = use_system_timestamp ?
                                                        arv_buffer_get_system_timestamp (buffer):
                                                        arv_buffer_get_timestamp (buffer);
                                        else
                                                end_time = use_system_timestamp ?
                                                        arv_buffer_get_system_timestamp (buffer):
                                                        arv_buffer_get_timestamp (buffer);
                                } else {
                                        success = FALSE;
                                }
                                arv_stream_push_buffer (stream, buffer);
                        }
                }
        }
        if (error == NULL)
                arv_camera_stop_acquisition (test_camera->camera, &error);

        g_clear_object (&stream);

        callback_init_success = callback_data.n_init == 1;
        callback_buffer_success =
                callback_data.n_start == callback_data.n_done &&
                callback_data.n_success >= n_expected_buffers;
        callback_exit_success = callback_data.n_exit == 1;

        success = success && callback_init_success && callback_buffer_success && callback_exit_success;

        message = g_strdup_printf ("%u/%u%s%s%s%s%s", n_completed_buffers, n_expected_buffers,
                                   callback_init_success ? "" : " cb_init_err",
                                   callback_buffer_success ? "" : " cb_buffer_err",
                                   callback_exit_success ? "" : " cb_exit_err",
                                   error != NULL ? " " : "",
                                   error != NULL ? error->message : "");
        arv_test_camera_add_result (test_camera, test_name, "BufferCheck",
                                    success && error == NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    message);
        g_clear_pointer (&message, g_free);

        frame_rate_success = FALSE;

        if (end_time > 0 && start_time > 0 && start_time != end_time) {
                double actual_frame_rate = 9 * 1e9 / (end_time - start_time);
                double frame_rate_error = fabs (actual_frame_rate - frame_rate) / frame_rate;

                if (frame_rate_error < 0.10) {
                        frame_rate_success = TRUE;
                        message = g_strdup_printf ("%.2f Hz", actual_frame_rate);
                } else
                        message = g_strdup_printf ("%.2f Hz (expected:%.2f Hz)", actual_frame_rate, frame_rate);
        } else {
                message = g_strdup_printf ("Missing timestamp information");
        }

        arv_test_camera_add_result (test_camera, test_name, "FrameRate",
                                    frame_rate_success ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    message);

        g_clear_error (&error);
        g_clear_pointer (&message, g_free);
}

static void
arv_test_multiple_acquisition_a (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        double frame_rate;
        gboolean use_system_timestamp;

        g_return_if_fail (ARV_IS_TEST (test));

        use_system_timestamp = arv_test_camera_get_key_file_boolean (test_camera, test, "UseSystemTimestamp", FALSE);
        frame_rate = arv_test_camera_get_key_file_double (test_camera, test, "FrameRateA", 10.0);

        _multiple_acquisition (test, test_name, test_camera, frame_rate, use_system_timestamp);
}

static void
arv_test_multiple_acquisition_b (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        double frame_rate;
        gboolean use_system_timestamp;

        g_return_if_fail (ARV_IS_TEST (test));

        use_system_timestamp = arv_test_camera_get_key_file_boolean (test_camera, test, "UseSystemTimestamp", FALSE);
        frame_rate = arv_test_camera_get_key_file_double (test_camera, test, "FrameRateB", 5.0);

        _multiple_acquisition (test, test_name, test_camera, frame_rate, use_system_timestamp);
}

static void
arv_test_software_trigger (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        GError* error = NULL;
        char *message = NULL;
        ArvStream *stream;
        _StreamCallbackData callback_data;
        unsigned int i;
        size_t payload_size;
        gboolean success = TRUE;
        gboolean software_trigger_support;
        guint n_completed_buffers = 0;
        guint n_expected_buffers = 5;
        double delay_s;

        g_return_if_fail (ARV_IS_TEST (test));

        software_trigger_support = arv_test_camera_get_key_file_boolean (test_camera, test,
                                                                         "SoftwareTriggerSupport", TRUE);

        if (!arv_camera_is_software_trigger_supported (test_camera->camera, &error)) {
                        arv_test_camera_add_result (test_camera, test_name, "NoSupport",
                                                    error == NULL && ! software_trigger_support ?
                                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                                    error != NULL ? error->message : NULL);
			g_clear_error (&error);
                        return;
        }

        delay_s = arv_test_camera_get_key_file_double (test_camera, test, "SoftwareTriggerWait", 0);

        arv_camera_set_acquisition_mode (test_camera->camera, ARV_ACQUISITION_MODE_CONTINUOUS, &error);
        if (error == NULL)
                arv_camera_set_trigger (test_camera->camera, "Software", &error);

        if (error != NULL) {
                arv_test_camera_add_result (test_camera, test_name, "BufferCheck",
                                            ARV_TEST_STATUS_FAILURE, error->message);
                g_clear_error (&error);
                return;
        }

        callback_data.n_init = 0;
        callback_data.n_start = 0;
        callback_data.n_done = 0;
        callback_data.n_success = 0;
        callback_data.n_exit = 0;
        stream = arv_camera_create_stream (test_camera->camera, _stream_calback, &callback_data, NULL, &error);
        if (error == NULL)
                payload_size = arv_camera_get_payload (test_camera->camera, &error);
        if (error == NULL) {
                for (i = 0 ; i < 2; i++)
                        arv_stream_push_buffer (stream, arv_buffer_new (payload_size, FALSE));
        }
        if (error == NULL)
                arv_camera_start_acquisition (test_camera->camera, &error);

        for (i = 0 ; i < n_expected_buffers && error == NULL && success; i++) {
                ArvBuffer *buffer;

                if (error == NULL) {
                        if (i > 0)
                                g_usleep (1000000.0 * delay_s);
                        arv_camera_software_trigger (test_camera->camera, &error);
                }

                if (error == NULL) {
                        buffer = arv_stream_timeout_pop_buffer (stream, 500000);
                        if (buffer == NULL)
                                success = FALSE;
                        else {
                                if (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_SUCCESS) {
                                        n_completed_buffers++;
                                } else {
                                        success = FALSE;
                                }
                                arv_stream_push_buffer (stream, buffer);
                        }
                }
        }

        if (error == NULL)
                arv_camera_stop_acquisition (test_camera->camera, &error);

        g_clear_object (&stream);

        success = success &&
                callback_data.n_init == 1 &&
                callback_data.n_start == callback_data.n_done &&
                callback_data.n_success == n_expected_buffers &&
                callback_data.n_exit == 1;

        message = g_strdup_printf ("%u/%u%s%s", n_completed_buffers, n_expected_buffers,
                                   error != NULL ? " " : "",
                                   error != NULL ? error->message : "");
        arv_test_camera_add_result (test_camera, test_name, "BufferCheck",
                                    success && error == NULL ? ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                    message);
        g_clear_pointer (&message, g_free);

        g_clear_error (&error);
}

static void
arv_test_gige_vision (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        GError *error = NULL;
        gint expected_n_stream_channels;
        gint n_stream_channels;
        gint expected_n_network_interfaces;
        gint n_network_interfaces;
        char *message;

        g_return_if_fail (ARV_IS_TEST (test));

        if (!arv_camera_is_gv_device (test_camera->camera))
                return;

        expected_n_network_interfaces = arv_test_camera_get_key_file_int64 (test_camera, test, "NNetworkInterfaces", 1);

        n_network_interfaces = arv_camera_gv_get_n_network_interfaces (test_camera->camera, &error);

        if (error != NULL)
                message = g_strdup_printf("%s", error->message);
        else
                message = g_strdup_printf("%d", n_network_interfaces);
        arv_test_camera_add_result (test_camera, test_name, "NNetworkInterfaces",
                                    n_network_interfaces == expected_n_network_interfaces && error == NULL ?
                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE, message);
        g_clear_pointer(&message, g_free);

        expected_n_stream_channels = arv_test_camera_get_key_file_int64 (test_camera, test, "NStreamChannels", 1);

        n_stream_channels = arv_camera_gv_get_n_stream_channels (test_camera->camera, &error);

        if (error != NULL)
                message = g_strdup_printf("%s", error->message);
        else
                message = g_strdup_printf("%d", n_stream_channels);
        arv_test_camera_add_result (test_camera, test_name, "NStreamChannels",
                                    n_stream_channels == expected_n_stream_channels && error == NULL ?
                                    ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE, message);
        g_clear_pointer(&message, g_free);

        g_clear_error (&error);
        if (expected_n_stream_channels > 0) {
                arv_camera_gv_get_current_stream_channel (test_camera->camera, &error);
                arv_test_camera_add_result (test_camera, test_name, "StreamChannel",
                                            error == NULL ?  ARV_TEST_STATUS_SUCCESS : ARV_TEST_STATUS_FAILURE,
                                            error != NULL ? error->message : NULL);
        } else {
                arv_test_camera_add_result (test_camera, test_name, "StreamChannel",
                                            ARV_TEST_STATUS_IGNORED, NULL);
        }
        g_clear_error (&error);
}

static void
arv_test_usb3_vision (ArvTest *test, const char *test_name, ArvTestCamera *test_camera)
{
        g_return_if_fail (ARV_IS_TEST (test));

        if (!arv_camera_is_uv_device (test_camera->camera))
                return;
}

const struct {
        const char *name;
        void (*run) (ArvTest *test, const char *test_name, ArvTestCamera *test_camera);
        gboolean is_slow;
} tests[] = {
        {"Genicam",                     arv_test_genicam,               FALSE},
        {"Properties",                  arv_test_device_properties,     FALSE},
        {"MultipleAcquisitionA",        arv_test_multiple_acquisition_a,FALSE},
        {"SingleAcquisition",           arv_test_single_acquisition,    FALSE},
        {"SoftwareTrigger",             arv_test_software_trigger,      FALSE},
        {"MultipleAcquisitionB",        arv_test_multiple_acquisition_b,FALSE},
        {"Multipart",                   arv_test_multipart,             FALSE},
        {"Chunks",                      arv_test_chunks,                FALSE},
        {"GigEVision",                  arv_test_gige_vision,           FALSE},
        {"USB3Vision",                  arv_test_usb3_vision,           FALSE}
};

static gboolean
arv_test_run (ArvTest *test, unsigned int n_iterations,
              const char *camera_selection,
              const char *test_selection,
	      ArvUvUsbMode usb_mode,
              gboolean cache_check,
              gboolean packet_socket)
{
        GRegex *camera_regex;
        GRegex *test_regex;
	unsigned n_devices, i, j;
        gboolean success = TRUE;

        g_return_val_if_fail (ARV_IS_TEST (test), FALSE);

	arv_update_device_list ();
	n_devices = arv_get_n_devices ();

        printf ("Found %d device%s\n", n_devices, n_devices > 1 ? "s" : "");

        camera_regex = arv_regex_new_from_glob_pattern (camera_selection != NULL ? camera_selection : "*", TRUE);
        test_regex = arv_regex_new_from_glob_pattern (test_selection != NULL ? test_selection : "*", TRUE);

        for (j = 0; j < n_iterations; j++) {
                for (i = 0; i < n_devices; i++) {
                        const char *camera_id = arv_get_device_id (i);

                        if (g_regex_match (camera_regex, camera_id, 0, NULL)) {
                                ArvTestCamera* test_camera = NULL;
                                unsigned int j;

                                test_camera = arv_test_camera_new (camera_id, cache_check);

                                if (test_camera == NULL) {
                                        printf ("Failed to connect to '%s:%s'\n",
                                                arv_get_device_vendor (i), arv_get_device_model (i));
                                } else {
                                        printf ("Testing '%s:%s'\n",
                                                arv_get_device_vendor (i), arv_get_device_model (i));

					if (arv_camera_is_uv_device (test_camera->camera))
						arv_camera_uv_set_usb_mode (test_camera->camera, usb_mode);

                                        if (arv_camera_is_gv_device(test_camera->camera))
                                                arv_camera_gv_set_stream_options
                                                        (test_camera->camera,
                                                         packet_socket ?
                                                         ARV_GV_STREAM_OPTION_NONE :
                                                         ARV_GV_STREAM_OPTION_PACKET_SOCKET_DISABLED);

                                        for (j = 0; j < G_N_ELEMENTS (tests); j++) {
                                                if (g_regex_match (test_regex, tests[j].name, 0, NULL)) {

                                                        if (arv_test_camera_get_key_file_boolean (test_camera, test,
                                                                                                  tests[j].name, TRUE)) {
                                                                char *delay_name;
                                                                double delay;

                                                                delay_name = g_strdup_printf ("%sDelay", tests[j].name);
                                                                delay = arv_test_camera_get_key_file_double
                                                                        (test_camera, test, delay_name, 0);
                                                                g_usleep (1000000.0 * delay);
                                                                tests[j].run (test, tests[j].name, test_camera);
                                                                g_free (delay_name);
                                                        } else {
                                                                char *comment;

                                                                arv_test_camera_add_result (test_camera, tests[j].name,
                                                                                            "*", ARV_TEST_STATUS_IGNORED,
                                                                                            NULL);

                                                                comment = arv_test_camera_get_key_file_comment
                                                                        (test_camera, test,
                                                                         tests[j].name);

                                                                if (comment != NULL) {
                                                                        printf ("%s\n", comment);
                                                                        g_free (comment);
                                                                }
                                                        }
                                                }
                                        }

                                        if (cache_check) {
                                                guint64 n_cache_errors;
                                                char *comment = NULL;

                                                n_cache_errors = arv_test_camera_get_n_register_cache_errors (test_camera);

                                                if (n_cache_errors > 0)
                                                        comment = g_strdup_printf ("%" G_GUINT64_FORMAT " error(s)",
                                                                                   n_cache_errors);

                                                arv_test_camera_add_result (test_camera, "Genicam", "RegisterCache",
                                                                            n_cache_errors == 0 ?
                                                                            ARV_TEST_STATUS_SUCCESS :
                                                                            ARV_TEST_STATUS_FAILURE,
                                                                            comment);
                                                g_free (comment);
                                        }
                                }

                                g_clear_pointer (&test_camera, arv_test_camera_free);
                        }
                }
        }

        g_regex_unref (camera_regex);
        g_regex_unref (test_regex);

        return success;
}

static gboolean
arv_test_set_configuration (ArvTest *test, const char *path, GError **error)
{
        g_return_val_if_fail (ARV_IS_TEST (test), FALSE);
        g_return_val_if_fail (path != NULL, FALSE);

        g_key_file_free (test->key_file);
        test->key_file = g_key_file_new ();

        return g_key_file_load_from_file (test->key_file, path, G_KEY_FILE_KEEP_COMMENTS, error);
}

static char *arv_option_camera_selection = NULL;
static char *arv_option_test_selection = NULL;
static gint arv_option_n_iterations = 1;
static char *arv_option_configuration = NULL;
static char *arv_option_debug_domains = NULL;
static char *arv_option_uv_usb_mode = NULL;
static gboolean arv_option_cache_check = FALSE;
static gboolean arv_option_packet_socket = FALSE;
static gboolean arv_option_show_version = FALSE;

static const GOptionEntry arv_option_entries[] =
{
	{
		"name", 				'n', 0, G_OPTION_ARG_STRING,
		&arv_option_camera_selection, 		"Device selection",
		"<pattern>"
	},
	{
		"test", 				't', 0, G_OPTION_ARG_STRING,
		&arv_option_test_selection, 		"Test selection",
		"<pattern>"
	},
	{
		"configuration", 			'c', 0, G_OPTION_ARG_STRING,
		&arv_option_configuration, 		"Alternative configuration",
		"<path>"
	},
	{
		"iterations", 				'i', 0, G_OPTION_ARG_INT,
		&arv_option_n_iterations, 		"Number of test repetitions",
		"<n_iter>"
	},
	{
		"usb-mode",				's', 0, G_OPTION_ARG_STRING,
		&arv_option_uv_usb_mode,		"USB device I/O mode",
		"{sync|async}"
	},
        {
		"cache-check",				'a', 0, G_OPTION_ARG_NONE,
		&arv_option_cache_check,		"Register cache check",
		NULL
        },
	{
		"packet-socket",			'\0', 0, G_OPTION_ARG_NONE,
		&arv_option_packet_socket,		"Enable use of packet socket",
		NULL
	},
	{
		"debug", 				'd', 0, G_OPTION_ARG_STRING,
		&arv_option_debug_domains, 		NULL,
		"{<category>[:<level>][,...]|help}"
	},
	{
		"version", 			        'v', 0, G_OPTION_ARG_NONE,
		&arv_option_show_version,     	        "Show version",
                NULL
	},
	{ NULL }
};

static const char *description_content =
"arv-test is an automated test utility that tries to exercise most of the\n"
"Aravis functionalities. By default it runs all the tests on all the detected\n"
"devices, but devices and tests can be selected using a glob pattern.\n\n"
"A default configuration file is bundled in the executable. An alternative\n"
"one with entries specific to the camera you want to test can be specified.";

int
main (int argc, char **argv)
{
	GOptionContext *context;
	GError *error = NULL;
        ArvTest *test = NULL;
        gboolean success = TRUE;
	ArvUvUsbMode usb_mode;

	context = g_option_context_new (NULL);
        g_option_context_set_summary (context, "Automated test utillity.");
        g_option_context_set_description (context, description_content);
	g_option_context_add_main_entries (context, arv_option_entries, NULL);

	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		g_option_context_free (context);
		g_print ("Option parsing failed: %s\n", error->message);
		g_error_free (error);
		return EXIT_FAILURE;
	}

	g_option_context_free (context);

        if (arv_option_show_version) {
                printf ("%u.%u.%u\n",
                        arv_get_major_version (),
                        arv_get_minor_version (),
                        arv_get_micro_version ());
                return EXIT_SUCCESS;
        }

	if (!arv_debug_enable (arv_option_debug_domains)) {
		if (g_strcmp0 (arv_option_debug_domains, "help") != 0)
			printf ("Invalid debug selection\n");
		else
			arv_debug_print_infos ();
		return EXIT_FAILURE;
	}

        test = arv_test_new ();

        if (arv_option_configuration != NULL) {
                arv_test_set_configuration (test, arv_option_configuration, &error);
                if (error != NULL) {
                        printf ("Failed to open configuration file '%s': %s\n", arv_option_configuration,
                                error->message);
                        return EXIT_FAILURE;
                }
        }

	if (arv_option_uv_usb_mode == NULL)
		usb_mode = ARV_UV_USB_MODE_DEFAULT;
	else if (g_strcmp0 (arv_option_uv_usb_mode, "sync") == 0)
		usb_mode = ARV_UV_USB_MODE_SYNC;
	else if (g_strcmp0 (arv_option_uv_usb_mode, "async") == 0)
		usb_mode = ARV_UV_USB_MODE_ASYNC;
	else {
		printf ("Invalid USB device I/O mode\n");
		return EXIT_FAILURE;
	}

        if (!arv_test_run (test,
                           arv_option_n_iterations,
                           arv_option_camera_selection,
                           arv_option_test_selection,
                           usb_mode,
                           arv_option_cache_check,
                           arv_option_packet_socket))
                success = FALSE;

        g_clear_object (&test);

	arv_shutdown ();

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
