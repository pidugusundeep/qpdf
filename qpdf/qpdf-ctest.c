#include <qpdf/qpdf-c.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static qpdf_data qpdf = 0;

static void report_errors()
{
    while (qpdf_more_warnings(qpdf))
    {
	printf("warning: %s\n", qpdf_next_warning(qpdf));
    }
    while (qpdf_more_errors(qpdf))
    {
	printf("error: %s\n", qpdf_next_error(qpdf));
    }
}

static void test01(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    printf("version: %s\n", qpdf_get_pdf_version(qpdf));
    printf("linearized: %d\n", qpdf_is_linearized(qpdf));
    printf("encrypted: %d\n", qpdf_is_encrypted(qpdf));
    if (qpdf_is_encrypted(qpdf))
    {
	printf("user password: %s\n", qpdf_get_user_password(qpdf));
    }
    report_errors();
}

static void test02(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_set_suppress_warnings(qpdf, QPDF_TRUE);
    qpdf_read(qpdf, infile, password);
    if (qpdf_init_write(qpdf, outfile) == QPDF_SUCCESS)
    {
	qpdf_set_static_ID(qpdf, QPDF_TRUE);
	qpdf_write(qpdf);
    }
    report_errors();
}

static void test03(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_set_content_normalization(qpdf, QPDF_TRUE);
    qpdf_write(qpdf);
    report_errors();
}

static void test04(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_set_ignore_xref_streams(qpdf, QPDF_TRUE);
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_write(qpdf);
    report_errors();
}

static void test05(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_set_linearization(qpdf, QPDF_TRUE);
    qpdf_write(qpdf);
    report_errors();
}

static void test06(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_set_object_stream_mode(qpdf, QPDF_OBJECT_STREAM_GENERATE);
    qpdf_write(qpdf);
    report_errors();
}

static void test07(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_set_qdf_mode(qpdf, QPDF_TRUE);
    qpdf_write(qpdf);
    report_errors();
}

static void test08(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_set_qdf_mode(qpdf, QPDF_TRUE);
    qpdf_set_suppress_original_object_IDs(qpdf, QPDF_TRUE);
    qpdf_write(qpdf);
    report_errors();
}

static void test09(char const* infile,
		   char const* password,
		   char const* outfile)
{
    qpdf_read(qpdf, infile, password);
    qpdf_init_write(qpdf, outfile);
    qpdf_set_static_ID(qpdf, QPDF_TRUE);
    qpdf_set_stream_data_mode(qpdf, QPDF_STREAM_DATA_UNCOMPRESS);
    qpdf_write(qpdf);
    report_errors();
}

int main(int argc, char* argv[])
{
    char* whoami = 0;
    char* p = 0;
    int n = 0;
    char const* infile;
    char const* password;
    char const* outfile;
    void (*fn)(char const*, char const*, char const*) = 0;

    if ((p = strrchr(argv[0], '/')) != NULL)
    {
	whoami = p + 1;
    }
    else if ((p = strrchr(argv[0], '\\')) != NULL)
    {
	whoami = p + 1;
    }
    else
    {
	whoami = argv[0];
    }
    if (argc != 5)
    {
	fprintf(stderr, "usage: %s n infile password outfile\n", whoami);
	exit(2);
    }

    n = atoi(argv[1]);
    infile = argv[2];
    password = argv[3];
    outfile = argv[4];

    fn = ((n == 1) ? test01 :
	  (n == 2) ? test02 :
	  (n == 3) ? test03 :
	  (n == 4) ? test04 :
	  (n == 5) ? test05 :
	  (n == 6) ? test06 :
	  (n == 7) ? test07 :
	  (n == 8) ? test08 :
	  (n == 9) ? test09 :
	  0);

    if (fn == 0)
    {
	fprintf(stderr, "%s: invalid test number %d\n", whoami, n);
	exit(2);
    }

    qpdf = qpdf_init();
    fn(infile, password, outfile);
    qpdf_cleanup(&qpdf);
    assert(qpdf == 0);

    return 0;
}
