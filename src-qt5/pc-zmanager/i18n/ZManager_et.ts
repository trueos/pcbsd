<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>Dialoog</translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>Rakenda muudatused rekursiivselt allapoole</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Väli</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Väärtus</translation>
    </message>
    <message>
        <source>Source</source>
        <translation>Allikas</translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>Lisa kasutaja väli</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>Eemalda kasutaja väli</translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>Päri vanemalt</translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>Pööra muudatused tagasi</translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>Määrab, kuidas ACL-i kirjed päritakse, siis kui faile ja kaustu luuakse. Võimalikud valikud on
discard, noallow, restricted ja passthrough. Nende valikute kirjeldused on olemas ACL-i väljade juures.</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>Määrab, kuidas ACL-i kirjeid chmod-i kaudu muudetakse.
Võimalikud valikud on discard, groupmask ja passthrough.Nende valikute kirjeldused on olemas ACL-i väljade juures.</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>Määrab, kas failide ligipääsu aega uuendatakse, kui neid loetakse.
Selle väljalülitamine vähendab kirjutamistega seotud võrguliiklust ja võib
jõudlust oluliselt parandada, kuid võib segadusse ajada meiliprogramme ja
muid sarnaseid programme.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>Kirjutuskaitsega väli, mis näitab andmekogule ja selle alamatele kättesaadavat
kettaruumi arvestamata muid tegevusi salvestusruumis. Kuna kettaruum
on salvestusruumis jagatud, siis võib kättesaadav ruum olla piiratud füüsilise
ketta suuruse tõttu, samuti kvootide, reserveeritud ruumi ja teiste andmekogude
tõttu.</translation>
    </message>
    <message>
        <source>If this property is set to off, the file system cannot be mounted,
and is ignored by &apos;zfs mount -a&apos;.  Setting this property to off is
similar to setting the mountpoint property to none, except that the
dataset still has a normal mountpoint property, which can be inherited
Setting this property to off allows datasets to be used solely as a
mechanism to inherit properties. One example of setting canmount=off is
to have two datasets with the same mountpoint, so that the children of
both datasets appear in the same directory, but might have different
inherited characteristics.</source>
        <translation>Kui see on välja lülitatud, siis ei saa failisüsteemi haakida, samuti ei
mõju sellele käsk &quot;zfs mount -a&quot;. Välja lülitamisel on sarnane mõju,
kui määrata haakepunkti väljale väärtus &quot;none&quot;, ainult et andmekogul
on endiselt tavaline haakepunkti väli, mida on võimalik pärida.
Välja lülitamine lubab andmekogusid kasutada ainuüksi väljade
edasi pärandamise mehhanismina. Haakimise väljalülitamisel on
võimalik omada kahte andmekogu sama haakepunktiga nii, et mõlemad
andmekogud ilmuvad samas kaustas, kuid neil on erinevad päritud
omadused.</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>Määrab, millist kontrollsummat kasutada andmete õigsuse kontrollimiseks. Vaikimisi
on see väli sisse lülitatud (on), mis tähendab, et sobiv algoritm valitakse automaatselt
(hetkel fletcher4, aga see võib tulevikus muutuda). Välja lülitades ei kontrollita andmete
õigsust. Kontrollsummade välja lülitamine EI OLE soovitatav.</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>Lülitab sisse või välja andmekogu andmete pakkimise. Võimalikud väärtused:on, off, lzjb, zle, gzip, gzip-N. Hetkel on valikud lzjb, gzip ja gzip-N sama mõjuga kui on.
Andmete pakkimise sisselülitamisel failisüsteemis, kus on juba andmeid, pakitakse
kokku ainult uued andmed. Olemasolevaid andmeid ei pakita kokku.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>Kirjutuskaitsega väli, mis näitab selle andmekogu andmete pakkimise tihedust.
Pakkimise saab sisse lülitada käsuga &quot;zfs set compression=on&quot;.
Väärtus väljendab viidatud failide loogilise ja füüsilise suuruse suhet.
Selle hulka kuulub pakkimisest otseselt võidetud kettaruum.</translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation type="unfinished">Määrab, mitu koopiat failisüsteemi kohta tehakse kasutaja andmetest.
Need koopiad tehakse lisaks salvestusruumi tasemel tehtud koopiatele.
</translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>Kirjutuskaitsega väli, mis näitab kuupäeva ja kellaaega, millal andmekogu loodi.</translation>
    </message>
    <message>
        <source>Configures deduplication for a dataset. The default value is off.
The default deduplication checksum is sha256 (this may change in the
future).  When dedup is enabled, the checksum defined here overrides
the checksum property. Setting the value to verify has the same
effect as the setting sha256,verify.
If set to verify, ZFS will do a byte-to-byte comparsion in case of
two blocks having the same signature to make sure the block contents
are identical.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>FreeBSD ei toeta hetkel välja devices.</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>Määrab, kas protsesse on lubatud käivitada sellelt failisüsteemilt.
Vaikimisi on see lubatud.</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>Kirjutuskaitsega väli, mis näitab, kas süsteem, kloon või hetktõmmis onparajasti haagitud.
</translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation>Määrab selle failisüsteemi haakepunkti. Kui haakepunkti muudetakse, siis failisüsteem
ja selle alamad haagitakse lahti. Kui uus väärtus on legacy, siis jäävad need haakimata.
Muidu haagitakse need automaatselt uuesti uude asukohta, kui välja väärtus oli varasemalt
legacy või none või kui failisüsteem oli haagitud enne haakepunkti muutmist.
Lisaks sellele jagatakse jagatud kaustad uues asukohas.</translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation>Määrab, mida hoitakse peamises vahemälus (ARC). Võimalikud väärtused on
all, none ja metadata. Kui väärtus on all, siis hoitakse vahemälus nii kasutaja
andmeid kui ka metaandmeid. Kui väärtus on metadata, siis hoitakse vahemälus
ainult metaandmeid. Kui väärtus on none, siis ei hoita vahemälus ei kasutajate
andmeid ega metaandmeid.</translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation>Kirjutuskaitsega väli, mis näitab kloonitud failisüsteemide või köidete puhul
hetktõmmist, mille põhjal kloon loodi. Nii kaua, kuni originaalist on olemas
kloone, ei saa originaali kustutada (isegi valikutega -r või -f).Failisüsteemide päritolu on none, kui need ei ole kloonitud.</translation>
    </message>
    <message>
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation>Piirab kettaruumi, mida andmekogu ja selle järglased kasutada saavad.
Väli määrab range piiri kettaruumi kasutusele, sealhulgas järglastele, nt
failisüsteemidele ja hetktõmmistele. Kvoodi määramine andmekogule,
millel juba kvoot olemas on, ei kirjuta üle kõrgemalt määratud sätteid,
vaid rakendab kvoodi lisaks olemasolevale.</translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>Määrab selle, kas andmekogusid saab muuta. Kui see väli on &quot;on&quot;, 
siis muudatusi teha ei saa.</translation>
    </message>
    <message>
        <source>Specifies a suggested block size for files in the file system. This
property is designed solely for use with database workloads that
access files in fixed-size records.  ZFS automatically tunes block
sizes according to internal algorithms optimized for typical access
patterns.
Use of this property for general purpose file systems is strongly
discouraged, and may adversely affect performance.
The size specified must be a power of two greater than or equal to
512 and less than or equal to 128 Kbytes.</source>
        <translation>Määrab soovitatava ploki suuruse failidele failisüsteemis. See on
mõeldud ainult andmebaasidele, kus failidele toimub ligipääs
kindla suurusega plokkide kaupa. ZFS muudab plokkide suurusi
automaatselt vastavalt sisemiste algoritmide poolt leitud
tüüpilistele kasutusmustritele.
Üldotstarbeliste süsteemide jaoks ei ole selle välja kasutamine 
soovitatav. See võib jõudlusele halvasti mõjuda.
Ploki suurus peab olema kahe aste ning peab olema vahemikus
512 baiti ja 128 kilobaiti (kaasa arvatud).</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation>Kirjutuskaitsega väli, mis näitab andmekogule kättesaadavate andmete
mahtu. Need andmed võivad, aga ei pruugi olla jagatud teiste
andmekogudega.</translation>
    </message>
    <message>
        <source>Sets the amount of disk space that a dataset can consume. This property
enforces a hard limit on the amount of space used. This hard limit does
not include disk space used by descendents, such as snapshots and clones.</source>
        <translation>Piirab kettaruumi, mida andmekogu ja selle järglased kasutada saavad.
Väli määrab range piiri kettaruumi kasutusele, kuid sinna alla ei kuulu
järglased, nt hetktõmmised ja kloonid.</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space that is guaranteed to a dataset, not
including descendents, such as snapshots and clones. When the amount of disk
space used is below this value, the dataset is treated as if it were taking up
the amount of space specified by refreservation. The refreservation reservation
is accounted for in the parent dataset&apos;s disk space used, and counts against
the parent dataset&apos;s quotas and reservations.
If refreservation is set, a snapshot is only allowed if enough free pool space
is available outside of this reservation to accommodate the current number of
referenced bytes in the dataset.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space guaranteed to a dataset and its descendents.
When the amount of disk space used is below this value, the dataset is treated as if
it were using the amount of space specified by its reservation. Reservations are
accounted for in the parent dataset&apos;s disk space used, and count against the parent
dataset&apos;s quotas and reservations.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,
and metadata. If set to all, both user data and metadata are cached. If set to none,
neither user data nor metadata is cached. If set to metadata, only metadata is cached.</source>
        <translation>Määrab, mida hoitakse sekundaarses vahemälus (L2ARC). Võimalikud väärtused on
all, none ja metadata. Kui väärtus on all, siis hoitakse vahemälus nii kasutaja
andmeid kui ka metaandmeid. Kui väärtus on metadata, siis hoitakse vahemälus
ainult metaandmeid. Kui väärtus on none, siis ei hoita vahemälus ei kasutajate
andmeid ega metaandmeid.</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>Määrab, kas setuid-bitti failisüsteemis arvestatakse.</translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>Määrab, kas failisüsteem on kättesaadav NFS-i kaudu.
Kui väärtus on &quot;on&quot;, siis käivitatakse käsk &quot;zfs share&quot; ilma
lisavalikuteta. Vastasel juhul pannakse käsu lõppu valikud,
mis on selle väljaga määratud.</translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>Väljal sharesmb ei ole FreeBSD-s mõju.</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>Määrab, kas kausta &quot;.zfs&quot; näidatakse failisüsteemi juurkaustas.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>Kirjutuskaitsega väli, mis näitab andmekogu tüüpi: failisüsteem (failisüsteem või kloon), köide või hetktõmmis.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>Kirjutuskaitsega väli, mis näitab andmekogu ja selle järglaste poolt kasutatud kettaruumi.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>Kirjutuskaitsega väli, mis näitab kettaruumi, mida selle andmekogu alamad
kasutavad. See on ruum, mida saaks vabastada, kui kõik selle andmekogu
alamad hävitada.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation type="unfinished">Kirjutuskaitsega väli, mis näitab andmekogu poolt kasutatud kettaruumi.
See on ruum, mida saaks vabastada, kui andmekogu hävitada, pärast seda,
kui hetktõmmised eemaldada.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation>Kirjutuskaitsega väli, mis näitab, kui palju kettaruumi kasutab andmekogu hetktõmmis.
See on kettaruum, mis vabaneks, kui kõik andmekogu hetktõmmised hävitataks.
See väärtus ei pruugi olla hetktõmmiste kasutatud kettaruumi väljade summa, kuna
hetktõmmised võivad kettaruumi jagada.</translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation>Määrab kettal oleva failisüstemi versiooni, mis on sõltumatu salvestusruumi
versioonist. Selle välja väärtus peab olema suurem toetatud tarkvara
väljalaske versioonist.</translation>
    </message>
    <message>
        <source>For volumes, specifies the logical size of the volume.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation>Näitab, kas andmekogu on vanglasse lisatud.</translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>FreeBSD ei toeta seda välja.</translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>Failisüsteem on märgitud hävitamiseks pärast lahti haakimist.</translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation>Selle andmekogu poolt viidatud andmete pakkimistihedus
väljendatuna kordajana.</translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation>Viidatud kettaruumi maht, mis on andmekogusse kirjutatud alates
eelmise hetktõmmise tegemisest.</translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Provide a hint to ZFS about handling of synchronous requests in this
dataset.  If logbias is set to latency (the default), ZFS will use
pool log devices (if configured) to handle the requests at low
latency. If logbias is set to throughput, ZFS will not use configured
pool log devices.  ZFS will instead optimize synchronous operations
for global pool throughput and efficient use of resources.</source>
        <translation type="unfinished">Vihjab ZFS-ile, et selles andmekogus töödeldakse sünkroonseid päringuid.
Kui välja logbias väärtus on latency (vaikimisi on), siis kasutab ZFS
andmekogu logimisseadmeid (kui need on seadistatud), et töödelda
päringuid madala latentsiga. Kui välja logbias väärtus on throughput,
siis ei kasuta ZFS seadistatud andmekogu logimisseadmeid. Selle asemel
optimeerib ZFS sünkroonsed tegevused läbilaskevõime suurendamiseks
ja ressursside efektiivseks kasutamseks.</translation>
    </message>
    <message>
        <source>Controls the behavior of synchronous requests (e.g.  fsync(2),
O_DSYNC). This property accepts the following values:
standard  This is the POSIX specified behavior of ensuring all
          synchronous requests are written to stable storage and
          all devices are flushed to ensure data is not cached by
          device controllers (this is the default).
always    All file system transactions are written and flushed
          before their system calls return. This has a large
          performance penalty.
disabled  Disables synchronous requests. File system transactions
          are only committed to stable storage periodically. This
          option will give the highest performance.  However, it
          is very dangerous as ZFS would be ignoring the synchro-
          nous transaction demands of applications such as data-
          bases or NFS.  Administrators should only use this
          option when the risks are understood.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property cannot be changed after the system is created.</source>
        <translation>Seda välja ei saa muuta pärast süsteemi loomist.</translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>Kasutaja väli</translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>Tundmatu väli või ei ole selle kohta infot saadaval.
Rohkem infot leiab ZFS-i kasutusjuhendist.</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation>[Vigane]</translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation>[Päritud]</translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation>[Lisatud]</translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation>[Muudetud]</translation>
    </message>
    <message>
        <source>User property</source>
        <translation>Kasutaja väli</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Seadme menüü</translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation>Lisa uus kasutaja väli</translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation>Kustuta kasutaja väli</translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation>Dialoog</translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation>Seade, mida haakida:</translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation>(Seade)</translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation>Haakimise asukoht:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation>Vali kaust</translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation>Dialoog</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Salvestusruumi nimi:</translation>
    </message>
    <message>
        <source>Do not mount any filesystems</source>
        <translation>Ära haagi ühtegi failisüsteemi</translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation>Kirjutuskaitsega salvestusruum</translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation>Importimise valikud:</translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation type="unfinished">Haagi alternatiivse juurkaustaga:</translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation>Haagi kõik failisüsteemid automaatselt</translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation>Käitumine haakimisel:</translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation>Vii tegevus vea korral lõpuni (kasuta ettevaatlikult)</translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation>Dialoog</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>Valitud</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Salvestusruumi nimi:</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>Seadistamise tüüp:</translation>
    </message>
    <message>
        <source>Striped</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Peegel</translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation type="unfinished">Raid-Z (ühekordne paarsus)</translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>Raid-Z2 (topeltpaarsus)            </translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>Raid-Z3 (kolmekordne paarsus)</translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>Vahemälu</translation>
    </message>
    <message>
        <source>Log</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>Varu</translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>Vea kirjeldus siia</translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>Valida tuleb täpselt %1 seadet</translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>Valida tuleb vähemalt üks seade</translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>Lisa partitsioon</translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation type="unfinished">Lisa uus lõik/partitsioon</translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 GB</translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>Seade:</translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>ada0</translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>Suurim kättesaadav vaba plokk:</translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>Suurus kokku:</translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Designation</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>Algussektor</translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>Sektoreid</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Suurus</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Värv</translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>Uue partitsiooni suurus:</translation>
    </message>
    <message>
        <source>bytes</source>
        <translation>baiti</translation>
    </message>
    <message>
        <source>kB</source>
        <translation>kB</translation>
    </message>
    <message>
        <source>MB</source>
        <translation>MB</translation>
    </message>
    <message>
        <source>GB</source>
        <translation>GB</translation>
    </message>
    <message>
        <source>TB</source>
        <translation>TB</translation>
    </message>
    <message>
        <source>Partition type:</source>
        <translation>Partitsiooni tüüp:</translation>
    </message>
    <message>
        <source>freebsd-zfs</source>
        <translation>freebsd-zfs</translation>
    </message>
    <message>
        <source>freebsd-ufs</source>
        <translation>freebsd-ufs</translation>
    </message>
    <message>
        <source>freebsd</source>
        <translation>freebsd</translation>
    </message>
    <message>
        <source>freebsd-boot</source>
        <translation>freebsd-boot</translation>
    </message>
    <message>
        <source>freebsd-swap</source>
        <translation>freebsd-swap</translation>
    </message>
    <message>
        <source>bios-boot</source>
        <translation>bios-boot</translation>
    </message>
    <message>
        <source>efi</source>
        <translation>efi</translation>
    </message>
    <message>
        <source>Create and initialize a new filesystem</source>
        <translation>Loo ja vorminda uus failisüsteem</translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** VABA **</translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>Vaba ruumi ei ole!</translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>Muuda salvestusruumi väljasid</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Väli</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Väärtus</translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>Salvestusruumi kogumaht.</translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>Salvestusruumi kasutus protsentuaalselt.</translation>
    </message>
    <message>
        <source>Alternate root directory. If set, this directory is prepended to any
mount points within the pool. This can be used when examining an
unknown pool where the mount points cannot be trusted, or in an
alternate boot environment, where the typical paths are not valid.
altroot is not a persistent property. It is valid only while the
system is up.  Setting altroot defaults to using cachefile=none, though
this may be overridden using an explicit setting.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation>Salvestusruumi hetkeseisund.</translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>Salvestusruumi unikaalne identifikaator.</translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation>Salvestusruumi hetkel kettal olev versioon. Seda saab suuurendada, aga
mitte kunagi vähendada. Salvestusruume uuendatakse eelistatult käsuga
&quot;zpool upgrade&quot;, kuigi seda välja saab kasutada siis, kui tagasiühilduvuseks
on vaja kindlat versiooni. Selle välja väärtus on vahemikus 1 kuni väärtus,
mille tagastab &quot;zpool upgrade -v&quot;.</translation>
    </message>
    <message>
        <source>Identifies the default bootable dataset for the root pool. This
property is expected to be set mainly by the installation and upgrade
programs.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether a non-privileged user is granted access based on the
dataset permissions defined on the dataset. See zfs(8) for more
information on ZFS delegated administration.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls automatic device replacement. If set to &quot;off&quot;, device
replacement must be initiated by the administrator by using the
&quot;zpool replace&quot; command. If set to &quot;on&quot;, any new device, found in the
same physical location as a device that previously belonged to the
pool, is automatically formatted and replaced. The default behavior
is &quot;off&quot;.  This property can also be referred to by its shortened
column name, &quot;replace&quot;.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls automatic pool expansion when the underlying LUN is grown.
If set to &quot;on&quot;, the pool will be resized according to the size of the
expanded device. If the device is part of a mirror or raidz then all
devices within that mirror/raidz group must be expanded before the
new space is made available to the pool. The default behavior is
&quot;off&quot;.  This property can also be referred to by its shortened column
name, expand.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls the location of where the pool configuration is cached.
Discovering all pools on system startup requires a cached copy of the
configuration data that is stored on the root file system. All pools
in this cache are automatically imported when the system boots. Some
environments, such as install and clustering, need to cache this
information in a different location so that pools are not automatically
imported. Setting this property caches the pool configuration
in a different location that can later be imported with &quot;zpool import
-c&quot;.  Setting it to the special value &quot;none&quot; creates a temporary pool
that is never cached, and the special value &apos;&apos; (empty string) uses
the default location.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>If set to on, pool will be imported in read-only mode with the
following restrictions:
  *   Synchronous data in the intent log will not be accessible
  *   Properties of the pool can not be changed
  *   Datasets of this pool can only be mounted read-only
  *   To write to a read-only pool, a export and import of the pool
      is required.
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls the system behavior in the event of catastrophic pool failure
This condition is typically a result of a loss of connectivity
to the underlying storage device(s) or a failure of all devices
within the pool. The behavior of such an event is determined as follows
wait
Blocks all I/O access until the device connectivity is recovered
and the errors are cleared. This is the default behavior
continue
Returns EIO to any new write I/O requests but allows reads to
any of the remaining healthy devices. Any write requests that
have yet to be committed to disk would be blocked.

panic
Prints out a message to the console and generates a system
crash dump.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DialogfsCreate</name>
    <message>
        <source>New Filesystem</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>none</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>legacy</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[path]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>formD</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>formKCF</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Copies =</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>off</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>on</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>lzjb</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>zle</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-1</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-2</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-3</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-4</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-5</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-6</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-7</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-8</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-9</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>ZManagerWindow</name>
    <message>
        <source>PCBSD Disk Manager</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Disks</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>State</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Status:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>poolname</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Status</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>No disk</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Available</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Offline</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Online</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Removed</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mount</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Error report</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Important information</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source> of </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>New name</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
Proceed with recovery?</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>An error was detected while executing &apos;%1&apos;:

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An error was detected while executing &apos;zpool&apos;:

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An error was detected while executing &apos;zfs&apos;:

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause data loss.

You are about to perform the following operation(s):

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>

Are you sure you want to proceed?

</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
