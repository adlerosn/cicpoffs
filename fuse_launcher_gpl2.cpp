/* 
 * CIOPFS - The Case Insensitive On Purpose Filesystem for FUSE
 *
 * (c) 2008 Marc Andre Tanner <mat at brain-dump dot org>
 * (c) 2001-2007 Miklos Szeredi <miklos@szeredi.hu>
 *
 * This program can be distributed under the terms of the GNU GPLv2.
 *
 * How it works:
 * 	Before any operation takes place all filenames are
 * 	converted to lower case. The original filenames are stored
 * 	in extended attributes named user.filename. This value
 * 	is returned upon request.
 *
 * 	Files or directories which aren't all lowercase in the
 * 	underlying file system are ignored. You should probably
 * 	start with an empty data directory and copy your contents
 * 	over.
 *
 * Requirements:
 * 	In order to compile ciopfs, you will need both
 * 	libfuse and libattr. Furthermore if you want a case
 * 	preserving file system you have to make sure that the
 * 	underlying file system supports extended attributes
 * 	(for example for ext{2,3} you need a kernel with
 * 	CONFIG_EXT{2,3}_FS_XATTR enabled). You probably also
 * 	want to mount the underlying filesystem with the
 * 	user_xattr option which allows non root users to create
 * 	extended attributes.
 *
 * 	If you want to work with unicode characters within file
 * 	names, you will need libicu from www.icu-project.org.
 * 	Otherwise disable it in config.mk, the file system will
 * 	only work with ascii [a-zA-Z] file names.
 * 
 * ---------------------------------------------
 * 
 * (c) 2008 Marc Andre Tanner <mat at brain-dump dot org>
 * (c) 2001-2007 Miklos Szeredi <miklos@szeredi.hu>
 * 
 * This file is part of CIOPFS.
 * 
 * CIOPFS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * CIOPFS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with CIOPFS.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * ---------------------------------------------
 * 
 * CIOPFS links:
 *  https://www.brain-dump.org/projects/ciopfs/
 *  https://github.com/martanne/ciopfs/
 *  https://git.sr.ht/~martanne/ciopfs
 * 
 */

#include "cicpoffs.cpp"

enum {
	CICPOFFS_OPT_HELP,
	CICPOFFS_OPT_VERSION
};

static bool single_threaded = true;

static void usage(const char* name){
	fprintf(stderr, "usage: %s directory mountpoint [options]\n"
	                "\n"
			"Mounts the content of directory at mountpoint case-insensitively.\n"
			"\n"
			"general options:\n"
			"    -o opt,[opt...]        mount options\n"
			"    -h|--help              print help\n"
			"       --version           print version\n"
			"\n", name);
}

static int cicpoffs_opt_parse(void* data, const char* arg, int key, struct fuse_args* outargs){
	switch (key) {
		case FUSE_OPT_KEY_NONOPT:
			if (!read_source_directory) {
				if (!(read_source_directory = realpath(arg, NULL))) {
					fprintf(stderr, "%s", outargs->argv[0]);
					exit(1);
				}
				return 0;
			}
			return 1;
		case FUSE_OPT_KEY_OPT:
			if (arg[0] == '-') {
				switch (arg[1]) {
					case 'd':
					case 'f':
						logmsg = stderr_print;
				}
			} else if (!strcmp("allow_other", arg)) {
				single_threaded = (getuid() == 0);
			}
			return 1;
		case CICPOFFS_OPT_HELP:
			usage(outargs->argv[0]);
			fuse_opt_add_arg(outargs, "-ho");
			fuse_main(outargs->argc, outargs->argv, &operations, NULL);
			exit(0);
		case CICPOFFS_OPT_VERSION:
			fprintf(stderr, "%s: %s\nFUSE API version: %d\n", outargs->argv[0], VERSION, fuse_version());
			fuse_opt_add_arg(outargs, "--version");
			fuse_main(outargs->argc, outargs->argv, &operations, NULL);
			exit(0);
		default:
			fprintf(stderr, "see `%s -h' for usage\n", outargs->argv[0]);
			exit(1);
	}
	return 1;
}

static struct fuse_opt cicpoffs_opts[] = {
	FUSE_OPT_KEY("-h",        CICPOFFS_OPT_HELP),
	FUSE_OPT_KEY("--help",    CICPOFFS_OPT_HELP),
	FUSE_OPT_KEY("-V",        CICPOFFS_OPT_VERSION),
	FUSE_OPT_KEY("--version", CICPOFFS_OPT_VERSION),
	FUSE_OPT_END
};

int main(int argc, char** argv){
	argv0 = argv[0];
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	fuse_opt_parse(&args, &read_source_directory, cicpoffs_opts, cicpoffs_opt_parse);

	umask(0);
	return fuse_main(args.argc, args.argv, &operations, NULL);
}
