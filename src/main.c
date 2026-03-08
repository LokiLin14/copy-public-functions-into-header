#include "copy-declarations.h"

#define REQUIRED_ARGS \
    REQUIRED_STRING_ARG(source_file, "source", "Source .c file path") \
    REQUIRED_STRING_ARG(header_file, "header", "Header .h file path") \

#define BOOLEAN_ARGS \
    BOOLEAN_ARG(help, "-h", "Show help") \
    BOOLEAN_ARG(debug, "-d", "Dry run, print to stdout instead of overwriting header file")

#include "easy-args/includes/easyargs.h"

int main(int argc, char* argv[]) {
    args_t args = make_default_args();

    if (!parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return 1;
    }

    if (args.debug) {
        fprintf(stderr, "Adding public functions from %s to %s\n", args.source_file, args.header_file);
    }

    return copy_declarations(args.source_file, args.header_file, args.debug);
}
