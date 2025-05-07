#include "postgres.h"

#include "access/genam.h"
#include "access/htup_details.h"
#include "access/sysattr.h"
#include "access/table.h"
#include "access/xact.h"
#include "catalog/catalog.h"
#include "catalog/dependency.h"
#include "catalog/indexing.h"
#include "catalog/objectaccess.h"
#include "catalog/pg_constraint.h"
#include "catalog/pg_conkeypath.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_type.h"
#include "commands/defrem.h"
#include "commands/tablecmds.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/lsyscache.h"
#include "utils/rel.h"
#include "utils/syscache.h"

/*
    * CreateConKeyPathEntry
    *	Create a constraint key path table entry.
    *
    * Subsidiary records (such as triggers or indexes to implement the
    * constraint) are *not* created here.  But we do make dependency links
    * from the constraint to the things it depends on.
    *
    * The new constraint's OID is returned.
    */
Oid 
CreateConKeyPathEntry(Oid conoid, 
                      int16 conkey, 
                      int16 conkeypath)
{
    Relation    conkeypathRel;
    Oid         conkeypathoid;
    HeapTuple	tup;
    bool        nulls[Natts_pg_conkeypath];
    Datum       values[Natts_pg_conkeypath];


    conkeypathRel = table_open(ConKeyPathRelationId, RowExclusiveLock);

    conkeypathoid = GetNewOidWithIndex(conkeypathRel, ConkeypathOidIndexId, Anum_pg_conkeypath_oid);
    values[Anum_pg_conkeypath_oid - 1] = ObjectIdGetDatum(conkeypathoid);
    values[Anum_pg_conkeypath_conoid - 1] = ObjectIdGetDatum(conoid);
    values[Anum_pg_conkeypath_conkey - 1] = Int16GetDatum(conkey);
    values[Anum_pg_conkeypath_conkeypath - 1] = Int16GetDatum(conkeypath);
    nulls[Anum_pg_conkeypath_oid - 1] = false;
    nulls[Anum_pg_conkeypath_conoid - 1] = false;
    nulls[Anum_pg_conkeypath_conkey - 1] = false;
    nulls[Anum_pg_conkeypath_conkeypath - 1] = false;
    tup = heap_form_tuple(RelationGetDescr(conkeypathRel), values, nulls);
    CatalogTupleInsert(conkeypathRel, tup);

    table_close(conkeypathRel, RowExclusiveLock);

    return conkeypathoid;
}

void 
RemoveConKeyPathById(Oid conoid, int16 conkeypath)
{
    Relation conkeypathRel;
    HeapTuple tup;
    Form_pg_conkeypath conform;

    conkeypathRel = table_open(ConKeyPathRelationId, RowExclusiveLock);
    tup = SearchSysCache2(CONKEYPATHOID,
                          ObjectIdGetDatum(conoid),
                          Int16GetDatum(conkeypath));

    if (!HeapTupleIsValid(tup)) /* should not happen */
        return;

    conform = (Form_pg_conkeypath) GETSTRUCT(tup);
    
    CatalogTupleDelete(conkeypathRel, &tup->t_self);
    ReleaseSysCache(tup);
    table_close(conkeypathRel, RowExclusiveLock);

}