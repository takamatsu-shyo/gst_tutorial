#include <gst/gst.h>

int tutorial_main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *filter, *converter, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    // Init
    gst_init (&argc, &argv);

    // Create the elements
    source = gst_element_factory_make ("videotestsrc", "source");
    filter = gst_element_factory_make ("vertigotv", "filter");
    converter = gst_element_factory_make ("videoconvert", "converter");
    sink = gst_element_factory_make ("autovideosink", "sink");

    // Create the empty pipeline
    pipeline = gst_pipeline_new ("test-pipeline");

    if (!pipeline || !source || !filter || !converter || !sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline
    gst_bin_add_many (GST_BIN (pipeline), source, filter, converter, sink, NULL);
    if (!gst_element_link_many (source, filter, converter, sink, NULL)) {
        g_printerr ("Elements couldn't be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    // Modify the source's properties
    g_object_set (source, "pattern", 0, NULL);

    // Start playing
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    bus = gst_element_get_bus (pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
            GST_MESSAGE_ERROR| GST_MESSAGE_EOS);

    if (msg!=NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE (msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error (msg, &err, &debug_info);
                g_printerr ("Error received from elemet %s: %s\n",
                        GST_OBJECT_NAME (msg->src), err->message);
                g_printerr ("Debug info: %s\n",
                        debug_info ? debug_info : "none");
                g_clear_error (&err);
                g_free (debug_info);
                break;
            case GST_MESSAGE_EOS:
                g_print ("EOS reached\n");
                break;
            default:
                g_printerr ("Unknown message\n");
                break;
        }
        gst_message_unref (msg);
    }
    return 0;
}

int main (int argc, char *argv[]) {
    return tutorial_main (argc, argv);
}
