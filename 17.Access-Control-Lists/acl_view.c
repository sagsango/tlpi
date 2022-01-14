/* acl_view.c

   Display the access control list (ACL) on a file.

   Usage: acl_view [-d] file

   If the '-d' option is specified, then the default ACL is displayed (and
   'file' must be a directory), otherwise the access ACL is displayed.

   This program is Linux-specific. ACLs are supported since Linux 2.6.
   To build this program, you must have the ACL library (libacl) installed
   on your system.
*/
#ifdef __APPLE__
#include <sys/types.h>
#define	ACL_USER_OBJ		0x00000001
#define	ACL_USER		0x00000002
#define	ACL_GROUP_OBJ		0x00000004
#define	ACL_GROUP		0x00000008
#define	ACL_MASK		0x00000010
#define	ACL_OTHER		0x00000020
#define	ACL_OTHER_OBJ		ACL_OTHER
#define	ACL_EVERYONE		0x00000040
#define	ACL_WRITE		0x0002
#define	ACL_READ		0x0004
#define	ACL_PERM_NONE		0x0000
#else
#include <sys/libacl.h>
#endif
#include <sys/acl.h>
#include "ugid_functions.h"
#include "tlpi_hdr.h"
static void
usageError(char *progName)
{
    fprintf(stderr, "Usage: %s [-d] filename\n", progName);
    exit(EXIT_FAILURE);
}
int
main(int argc, char *argv[])
{
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag;
    uid_t *uidp;
    gid_t *gidp;
    acl_permset_t permset;
    char *name;
    int entryId, permVal, opt;

    type = ACL_TYPE_ACCESS;
    while ((opt = getopt(argc, argv, "d")) != -1) {
        switch (opt) {
        case 'd': type = ACL_TYPE_DEFAULT;      break;
        case '?': usageError(argv[0]);
        }
    }

		printf("Here, optind:%d, argc:%d\n",optind, argc);
    if (optind + 1 != argc)
        usageError(argv[0]);

    acl = acl_get_file(argv[optind], type);
    if (acl == NULL)
        errExit("acl_get_file");

    /* Walk through each entry in this ACL */

    for (entryId = ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY) {

        if (acl_get_entry(acl, entryId, &entry) != 1)
            break;                      /* Exit on error or no more entries */

        /* Retrieve and display tag type */

        if (acl_get_tag_type(entry, &tag) == -1)
            errExit("acl_get_tag_type");

        printf("%-12s", (tag == ACL_USER_OBJ) ?  "user_obj" :
                        (tag == ACL_USER) ?      "user" :
                        (tag == ACL_GROUP_OBJ) ? "group_obj" :
                        (tag == ACL_GROUP) ?     "group" :
                        (tag == ACL_MASK) ?      "mask" :
                        (tag == ACL_OTHER) ?     "other" : "???");

        /* Retrieve and display optional tag qualifier */

        if (tag == ACL_USER) {
            uidp = acl_get_qualifier(entry);
            if (uidp == NULL)
                errExit("acl_get_qualifier");

            name = userNameFromId(*uidp);
            if (name == NULL)
                printf("%-8d ", *uidp);
            else
                printf("%-8s ", name);

            if (acl_free(uidp) == -1)
                errExit("acl_free");

        } else if (tag == ACL_GROUP) {
            gidp = acl_get_qualifier(entry);
            if (gidp == NULL)
                errExit("acl_get_qualifier");

            name = groupNameFromId(*gidp);
            if (name == NULL)
                printf("%-8d ", *gidp);
            else
                printf("%-8s ", name);

            if (acl_free(gidp) == -1)
                errExit("acl_free");

        } else {
            printf("         ");
        }

        /* Retrieve and display permissions */

        if (acl_get_permset(entry, &permset) == -1)
            errExit("acl_get_permset");
#ifdef __APPLE__
        permVal = acl_get_perm_np(permset, ACL_READ);
#else
				permVal = acl_get_perm(permset, ACL_READ);
#endif

        if (permVal == -1)
            errExit("acl_get_perm - ACL_READ");
        printf("%c", (permVal == 1) ? 'r' : '-');
#ifdef __APPLE__
        permVal = acl_get_perm_np(permset, ACL_WRITE);
#else
				permVal = acl_get_perm(permset, ACL_WRITE);
#endif
        if (permVal == -1)
            errExit("acl_get_perm - ACL_WRITE");
        printf("%c", (permVal == 1) ? 'w' : '-');
#ifdef __APPLE__
				permVal = acl_get_perm_np(permset, ACL_EXECUTE);
#else
        permVal = acl_get_perm(permset, ACL_EXECUTE);
#endif
        if (permVal == -1)
            errExit("acl_get_perm - ACL_EXECUTE");
        printf("%c", (permVal == 1) ? 'x' : '-');

        printf("\n");
    }

    if (acl_free(acl) == -1)
        errExit("acl_free");

    exit(EXIT_SUCCESS);
}
