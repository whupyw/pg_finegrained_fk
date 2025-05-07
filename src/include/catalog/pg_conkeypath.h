#ifndef PG_CONKEYPATH_H
#define PG_CONKEYPATH_H

#include "catalog/genbki.h"
#include "catalog/pg_conkeypath_d.h"
#include "nodes/pg_list.h"

CATALOG(pg_conkeypath,6666,ConKeyPathRelationId)
{
    Oid			oid;			/* oid */
    Oid			conoid BKI_LOOKUP(pg_constraint);	/* OID of constraint */
    int16	    conkey;		/* key columns of the constraint */
    int16       conkeypath;	/* key columns path of the constraint */
} FormData_pg_conkeypath;

typedef FormData_pg_conkeypath *Form_pg_conkeypath;

DECLARE_UNIQUE_INDEX(pg_conkeypath_conoid_conkey_index, 6667, ConKeyPathConkeyIndexId, on pg_conkeypath using btree(conoid oid_ops, conkey int2_ops));
DECLARE_UNIQUE_INDEX_PKEY(pg_conkeypath_oid_index, 6668, ConkeypathOidIndexId, on pg_conkeypath using btree(oid oid_ops));


extern Oid CreateConKeyPathEntry(Oid conoid, 
                                 int16 conkey, 
                                 int16 conkeypath);

extern void RemoveConKeyPathById(Oid conoid, int16 conkeypath);

#endif                            /* PG_CONKEYPATH_H */