#include <string_view>

#include "../headers/args.hpp"

// set the members of the arguments struct appropriately to command line arguments
statusCode parse_args(arguments& args, int argc, char** argv)
{
	static std::map<std::string_view, argumentValues> argumentMap
	{
		{"-a", AUTHOR},
		{"--author", AUTHOR},
		{"-d", DATE},
		{"--date", DATE},
		{"-t", TITLE},
		{"--title", TITLE},
		{"-h", HELP},
		{"--help", HELP},
		{"-l", LANGUAGE},
		{"--language", LANGUAGE},
		{"--identifier", IDENTIFIER},
		{"-i", IDENTIFIER},
		{"--publisher", PUBLISHER},
		{"-p", PUBLISHER},
		{"-V", VERSION},
		{"--version", VERSION},
		{"-o", OUTFILE},
		{"-f", FORCE},
		{"--force", FORCE},
	};

	for (int i{0}; i < argc; ++i)
	{
		std::string_view arg{argv[i]};
		switch(argumentMap[arg])
		{
			case NOTOPTION:
				// assume the last command line argument is the path to the build dir.
				if (i != (argc - 1))
				{
					args.unknownOption = arg;
					return INVALID_OPTION;
				}

				// if the path is to a subdirectory add './' in front of its name
				if (arg[0] != '.' && arg[0] != '/')
				{
					args.path = "./" + std::string{arg.data()};
				}
				else
				{
					args.path = arg;
				}

				if (arg.back() != '/')
				{
					args.path += '/';
				}

				break;

			case AUTHOR:
				if (!is_argument(argv[i + 1]))
				{
					return ARG_REQUIRED;
				}

				args.author = argv[++i];
				break;

			case TITLE:
				if (!is_argument(argv[i + 1]))
				{
					return ARG_REQUIRED;
				}

				args.title = argv[++i];
				break;

			case DATE:
				if (!is_argument(argv[i + 1]))
				{
					args.requires_argument = arg;
					return ARG_REQUIRED;
				}

				args.date = argv[++i];
				break;

			case LANGUAGE:
				if (!is_argument(argv[i + 1]))
				{
					return ARG_REQUIRED;
				}

				args.language = argv[++i];
				break;

			case OUTFILE:
				if (!is_argument(argv[i + 1]))
				{
					return ARG_REQUIRED;
				}

				args.outfile = argv[++i];
				break;

			case IDENTIFIER:
				if (!is_argument(argv[i + 1]))
				{
					return ARG_REQUIRED;
				}

				args.identifier = argv[++i];
				break;

			case PUBLISHER:
				if (!is_argument(argv[i + 1]))
				{
					return ARG_REQUIRED;
				}

				args.publisher = argv[++i];
				break;

			case HELP:
				args.help = true;
				break;

			case VERSION:
				args.version = true;
				break;

			case FORCE:
				args.force = true;
		}
	}

	return NORMAL;
}

bool is_argument(std::string_view argument)
{
	// assume that if the next element in argv starts with a '-' assume it is a command
	return !(argument[0] == '-');
}
