#include <db.h>
#include <stdio.h>
#include <fcntl.h>
#define NAME      "Bill"
#define PHONE_NO          "123-4567"
#define DB_NAME   "phones"
main()
{
	/* TODO: 
		 This program was copied from oracle db,
		 which is similar to api aviable in linux

		 search in man page: dbopen
	*/

     DB *db;
     datum name = {NAME, sizeof (NAME)};
     datum put_phone_no = {PHONE_NO, sizeof (PHONE_NO)};
     datum get_phone_no;
     /* Open the database and store the record */
     db = dbm_open(DB_NAME, O_RDWR | O_CREAT, 0660);
     (void) dbm_store(db, name, put_phone_no, DBM_INSERT);
     /* Retrieve the record */
     get_phone_no = dbm_fetch(db, name);
     (void) printf("Name: %s, Phone Number: %s\n", name.dptr,
     get_phone_no.dptr);
     /* Close the database */
     dbm_close(db);
     return (0);
}
