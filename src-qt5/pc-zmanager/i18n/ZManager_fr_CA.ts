<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>Dialog     </translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>Appliquer les modifications de manière récursive à tous les descendants</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Propriété</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <source>Source</source>
        <translation>Source</translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>Ajouter la propriété de l&apos;utilisateur</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>Retirez la propriété de l&apos;utilisateur</translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>Hériter du parent</translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>Annuler les changements</translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>Contrôle des entrées ACL sont héritées lorsque les fichiers et les répertoires sont créés. Les valeurs sont
discard, noallow, restricted, and passthrough. Pour une description de ces valeurs, voir Propriétés ACL.</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>Contrôle le processus de modification des entrées ACL lors d&apos;une opération chmod.
Les valeurs possibles sont discard, groupmask et passthrough. Pour une description des
ces valeurs, voir Propriétés ACL.</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>Contrôle si le temps d&apos;accès aux fichiers est mis à jour lorsqu&apos;ils sont lus.
La désactivation de cette propriété évite hors production trafic d&apos;écriture lors de la lecture des fichiers et
peut entrainer des gains de performance significatifs, même si cela risque d&apos;induire les expéditeurs et
utilitaires similaires.    </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>Propriété en lecture seule indiquant la quantité d&apos;espace disque disponible pour un ensemble de données
et tous ses enfants, en supposant qu&apos;aucun autre activité dans le pool. Parce que l&apos;espace disque est
partagées au sein d&apos;un pool, l&apos;espace disponible peut être limitée par divers facteurs, y compris
taille du pool physique, les quotas, les réservations et autres ensembles de données au sein du pool.</translation>
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
        <translation>Si cette propriété est désactivée, le système de fichier ne peut pas être monté,
et est ignoré par &apos;zfs mount -a&apos;. La définition de cette propriété sur off est
similaire à définir la propriété mountpoint sur aucun, sauf que les
ensembles de données possède toujours une propriété mountpoint normale, ce qui peut être héritée
La définition de cette propriété sur off permet des ensembles de données qui seront utilisés uniquement comme un 
mécanisme d&apos;hériter des propriétés. Un exemple de mise en canmount = off est
d&apos;avoir deux ensembles de données avec le même point de montage, de sorte que les enfants de 
deux ensembles de données apparaissent dans le même répertoire, mais peuvent avoir différentes
hérité caractéristiques.</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>Contrôle la somme de contrôle permettant de vérifier l&apos;intégrité des données. La valeur par défaut
valeur est en marche, qui sélectionne automatiquement un algorithme approprié
(actuellement, fletcher4, mais cela pourrait changer dans les versions futures). Le
valeur hors intégrité désactive la vérification des données des utilisateurs. Désactivation
checksums n&apos;est pas une pratique recommandée.</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>Active ou désactive la compression de données. Les valeurs sont on, off, lzjb, zle, gzip, 
et gzip-N. Actuellement, la définition de cette propriété à lzjb, gzip ou gzip-N a le même 
effet que la définition de cette propriété sur. Activation de la compression sur un système de fichiers avec/ndonnées existantes uniquement la compression des nouvelles données. Les données existantes ne sont pas compressées.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>Propriété en lecture seule indiquant le ratio de compression obtenu pour un ensemble de données,
exprimée comme un multiplicateur. La compression peut être activée par la commande zfs set compression = on
commande de données. La valeur est calculée à partir de la taille logique de tous les fichiers et les
quantité de données physiques indiquée. Il comprend des économies explicites grâce à l&apos;utilisation de
la propriété de compression.</translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation>Définit le nombre de copies des données utilisateur par système de fichiers. Ces copies
s&apos;ajoutent à toute redondance de niveau pool. L&apos;espace disque utilisé par
plusieurs copies de données utilisateur est facturé pour le fichier correspondant et
DataSet et pénalise les quotas et les réservations. En outre, le
bien utilisé est mis à jour lorsque plusieurs copies sont activées. Envisager de mettre
cette propriété lorsque le système de fichier est créé parce que la modification de cette propriété
sur un système de fichiers existant affecte uniquement les données nouvellement écrites.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>Propriété en lecture seule identifiant la date et l&apos;heure qu&apos;un jeu de données a été créée. </translation>
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
        <translation>Configuration de la dé-duplication pour un ensemble de données. La valeur par défaut est off.
Le contrôle de dé-duplication par défaut est sha256 (cela pourrait changer à l&apos;
avenir). Quand dedup est activée, le contrôle défini ici remplace
la propriété de contrôle. Réglage de la valeur pour vérifier a la même 
effet que la mise en sha256, vérifiez.
Si elle est définie à vérifier, ZFS feront un comparsion octet par octet en cas de 
deux blocs ayant la même signature pour s&apos;assurer que le contenu du bloc
sont identiques.</translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>La propriété du dispositif n&apos;est actuellement pas pris en charge sur FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>Contrôle si les processus peuvent être exécutés à l&apos;intérieur de ce fichier
système. La valeur par défaut est activé.</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>Propriété en lecture seule qui indique que un système de fichiers, un clone ou un instantané
actuellement monté.</translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation>Contrôle le point de montage utilisé pour le système de fichiers. Lorsque la propriété mountpoint
est changé pour un système de fichiers, le système de fichiers et les descendants qui héritent de ce
point de montage sont démontés. Si la nouvelle valeur est héritage, alors ils restent démontés.
Sinon, ils sont automatiquement remontés au nouvel emplacement si la propriété était
précédemment héritage ou pas, ou s&apos;ils étaient montés avant que le bien a été modifiée.
En outre, les systèmes de fichiers partagés sont non partagées et partagées dans le nouvel emplacement.</translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation>Contrôle ce qui est mis en cache dans le cache principal (ARC). Les valeurs possibles sont tous,
none et metadata. Si réglé sur all, les données de l&apos;utilisateur et les métadonnées sont mises en cache. Si
définie sur none, ni les données d&apos;utilisateur ni les métadonnées sont mises en cache. Si réglé sur metadata, seules les
métadonnées sont mises en cache.</translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation>Propriété en lecture seule pour les systèmes de fichiers ou aux volumes clonés qui identifie
l&apos;instantané à partir duquel le clone a été créé. L&apos;origine ne peut pas être
détruit (même avec l&apos;option-r ou option-f) tant que le clone existe.
Les systèmes de fichiers non clonés ont une origine d&apos;aucun.</translation>
    </message>
    <message>
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation>Limite la quantité d&apos;espace disque un ensemble de données et ses descendants peuvent consommer.
Cette propriété impose une limite fixe à la quantité d&apos;espace disque utilisée,
y compris l&apos;espace utilisé par les descendants, tels que les systèmes de fichiers et les instantanés.
Fixation d&apos;un quota sur un descendant d&apos;un ensemble de données qui dispose déjà d&apos;un quota ne
le remplacement du quota de l&apos;ancêtre, mais impose plutôt une limite supplémentaire.</translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>Contrôle si un ensemble de données peut être modifié. Quand elle est activée,
pas de modifications peuvent être apportées. </translation>
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
        <translation>Indique une taille de bloc suggérée pour les fichiers du système de fichiers. Cette
propriété est conçu uniquement pour une utilisation avec des charges de travail de base de données
accès aux fichiers dans les dossiers de taille fixe. ZFS règle automatiquement bloc
tailles en fonction d&apos;algorithmes internes optimisés pour l&apos;accès typique
patterns.
L&apos;utilisation de cette propriété pour les systèmes de fichiers but est fortement
découragé, et peut nuire au rendement.
La taille spécifiée doit être une puissance de deux supérieure ou égale à
512 et inférieure ou égale à 128 kilo-octets.   </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation>Propriété en lecture seule qui indique la quantité de données accessible par un 
ensemble de données, ce qui pourrait ou ne pourrait pas être partagé avec d&apos;autres ensembles de données dans le répertoire
pool. </translation>
    </message>
    <message>
        <source>Sets the amount of disk space that a dataset can consume. This property
enforces a hard limit on the amount of space used. This hard limit does
not include disk space used by descendents, such as snapshots and clones.</source>
        <translation>Définit la quantité d&apos;espace disque qu&apos;un ensemble de données peut consommer. Cette propriété
impose une limite fixe à la quantité d&apos;espace utilisé. Cette limite maximale ne
ne pas inclure d&apos;espace disque utilisé par les descendants, notamment les instantanés et les clones. </translation>
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
        <translation>Fixe le montant minimum d&apos;espace disque qui est garanti pour un ensemble de données, pas
y compris les descendants, notamment les instantanés et les clones. Lorsque la quantité d&apos;espace disque
espace utilisé est inférieur à cette valeur, l&apos;ensemble de données est traitée comme si elle était prise de
la quantité d&apos;espace spécifiée par refreservation. La réservation refreservation
est pris en compte dans l&apos;espace disque de la base de données utilisée parent et chefs d&apos;accusation contre
Les quotas et réservations de jeux de données parent.
Si refreservation est définie, un instantané n&apos;est autorisé que si suffisamment d&apos;espace libre pool
est disponible à l&apos;extérieur de cette réserve pour accueillir le nombre actuel de
octets référencés dans le jeu de données.</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space guaranteed to a dataset and its descendents.
When the amount of disk space used is below this value, the dataset is treated as if
it were using the amount of space specified by its reservation. Reservations are
accounted for in the parent dataset&apos;s disk space used, and count against the parent
dataset&apos;s quotas and reservations.</source>
        <translation>Fixe le montant minimum d&apos;espace disque garanti à un ensemble de données et de ses descendants.
Lorsque la quantité d&apos;espace disque utilisée est inférieure à cette valeur, l&apos;ensemble de données est traitée comme si
il utilise la quantité d&apos;espace spécifiée par sa réserve. Les réservations sont
pris en compte dans l&apos;espace disque du parent données utilisées, et imputées sur le parent
Les quotas et les réservations des données.</translation>
    </message>
    <message>
        <source>Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,
and metadata. If set to all, both user data and metadata are cached. If set to none,
neither user data nor metadata is cached. If set to metadata, only metadata is cached.</source>
        <translation>Contrôle ce qui est mis en cache dans le cache secondaire (L2ARC). Les valeurs possibles sont all, none
et des métadonnées. Si réglé sur all, les données de l&apos;utilisateur et les métadonnées sont mises en cache. Si la valeur none,
ni les données d&apos;utilisateur ni les métadonnées sont mises en cache. S&apos;il est réglé sur metadata, seules les métadonnées sont mises en cache.</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>Contrôle si le bit setuid dans un système de fichiers.</translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>Contrôle si un système de fichiers est disponible via NFS et ce
options sont utilisées. Si elle est activée, la commande zfs share est exécutée
sans option. Sinon, la commande zfs share est exécutée avec
options équivalentes au contenu de cette propriété.</translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>La propriété sharesmb a actuellement aucun effet sur FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>Contrôle si le répertoire ZFS. Est caché ou visible dans la racine du système de fichiers.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>Propriété en lecture seule qui identifie le type de données comme système de fichiers (système de fichiers ou clone), le volume ou instantané.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>Propriété en lecture seule indiquant la quantité d&apos;espace disque consommé par un ensemble de données et de tous ses descendants.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>Propriété en lecture seule indiquant la quantité d&apos;espace disque qui est utilisé par
enfants de cet ensemble de données, qui serait libérée si tous les enfants du jeu de données
ont été détruites.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation>Propriété en lecture seule indiquant la quantité d&apos;espace disque qui est utilisé par
un ensemble de données lui-même, qui serait libérée si le jeu de données a été détruite, après
d&apos;abord détruire les instantanés et la suppression des réserves réservation.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation>Propriété en lecture seule indiquant la quantité d&apos;espace disque qui est utilisé par un
réservation situé sur un ensemble de données, qui serait libérée si la réservation était
éliminée.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation>Propriété en lecture seule indiquant la quantité d&apos;espace disque qui est consommé
par les instantanés d&apos;un ensemble de données. En particulier, c&apos;est la quantité d&apos;espace disque que
serait libérée si l&apos;ensemble des instantanés de ce jeu de données ont été détruits. Notez que
cette valeur n&apos;est pas simplement la somme des propriétés used des instantanés, car
espace peut être partagé par plusieurs instantanés.</translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation>Identifie la version sur disque d&apos;un système de fichiers, qui est indépendante de la
Version piscine. Cette propriété ne peut être réglé à une version plus récente est disponible
à partir de la version du logiciel pris en charge.</translation>
    </message>
    <message>
        <source>For volumes, specifies the logical size of the volume.</source>
        <translation>Pour les volumes, spécifie la taille logique du volume.</translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation>Pour les volumes, spécifie la taille de bloc du volume.</translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation>Indique si un ensemble de données a été ajoutée à un Jail.</translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>Cette propriété n&apos;est pas supportée sur FreeBSD.</translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>Le système du fichier est marqué pour être détruites après démontage.</translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation>Le taux de compression atteint pour l&apos;espace référencé de cette
ensemble de données, exprimée comme un multiplicateur.</translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation>La quantité d&apos;espace référencé par écrit à cette base de données depuis le
instantané précédent.</translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation>Cette propriété est définie sur le nombre d&apos;utilisateur tient sur ​​ce cliché.
</translation>
    </message>
    <message>
        <source>Provide a hint to ZFS about handling of synchronous requests in this
dataset.  If logbias is set to latency (the default), ZFS will use
pool log devices (if configured) to handle the requests at low
latency. If logbias is set to throughput, ZFS will not use configured
pool log devices.  ZFS will instead optimize synchronous operations
for global pool throughput and efficient use of resources.</source>
        <translation>Fournir une touche de ZFS sur le traitement des demandes synchrones dans cette
ensemble de données. Si logbias est fixé à latence (par défaut), ZFS vont utiliser
périphériques de journalisation de pool(s&apos;il est configuré) pour traiter les demandes à faible
latence. Si logbias est réglé sur le débit, ZFS n&apos;utiliseront pas configuré les
périphériques de journalisation de pool. ZFS seront plutôt optimiser les opérations synchrones
pour un débit de réserve globale et l&apos;utilisation efficace des ressources. </translation>
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
        <translation>Contrôle le comportement des requêtes synchrones (par exemple fsync (2),
O_DSYNC). Cette propriété accepte les valeurs suivantes:
classique, c&apos;est le comportement spécifié POSIX de sorte que tous les
requêtes synchrones sont écrits au stockage stable et
tous les appareils sont vidées de s&apos;assurer que les données ne sont pas mises en cache par
Les contrôleurs de périphériques (c&apos;est le défaut).
toujours Toutes les transactions du système de fichiers sont écrits et vidés
avant de leur système appelle retour. Cela a une grande
pénalité de performance.
désactivé désactive synchrone demandes. Transactions du système de fichiers
sont seulement engagés à stockage stable périodiquement. Cette
option donner la plus haute performance. Cependant, il
est très dangereux car ZFS ne tiendrait pas compte de la synchro-
demandes de transactions d&apos;applications telles que des données 
des bases ou NFS. Les administrateurs ne doivent utiliser cette
option lorsque les risques sont connus.</translation>
    </message>
    <message>
        <source>This property cannot be changed after the system is created.</source>
        <translation>Cette propriété ne peut pas être modifié après que le système est créé.</translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>Propriété de l&apos;utilisateur</translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>Propriété inconnu ou information non disponible.
Lisez le manuel ZFS pour plus d&apos;informations.</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation>[Ajouter]</translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation>[Hériter]</translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation>[Ajouter]</translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation>[Modifié]</translation>
    </message>
    <message>
        <source>User property</source>
        <translation>Propriété de l&apos;utilisateur</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Menu du périphérique</translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation>Ajouter une nouvelle propriété utilisateur</translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation>Supprimer la propriété de l&apos;utilisateur</translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation>Dispositif à monter:</translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation>(Dispositif)</translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation>Montez l&apos;emplacement:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation>Sélectionner le répertoire</translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Nom du Pool:</translation>
    </message>
    <message>
        <source>Do not mount any filesystems</source>
        <translation>Ne montez pas les systèmes de fichiers</translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation>Lecture seule pool</translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation>Options d&apos;importation:</translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation>Monter avec l&apos;alternative root:</translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation>Monter les systèmes de fichier automatiquement</translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation>Comportement de montage:</translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation>Forcer l&apos;opération en cas d&apos;erreur (utiliser avec prudence)</translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Nom Pool:</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>Type de configuration:</translation>
    </message>
    <message>
        <source>Striped</source>
        <translation>Rayé</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Miroir</translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation>RAID-Z (parité simple)</translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>RAID-Z (parité simple)</translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>Raid-Z3 (triple parité)</translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>Cache</translation>
    </message>
    <message>
        <source>Log</source>
        <translation>Log</translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>Épargnez</translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>Description de l&apos;erreur ici</translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation>Vous devez fournir un nom de pool valide</translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>Vous devez sélectionner exactement 1% périphériques</translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>Vous devez sélectionner au moins un dispositif</translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>Ajouter une Partition</translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation>Ajouter une nouvelle tranche/partition</translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 GB</translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>Dispositif</translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>ada0</translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>Plus grand bloc libre disponible:</translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>Taille totale:</translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation>Configuration actuelle:</translation>
    </message>
    <message>
        <source>Designation</source>
        <translation>Désignation</translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>Commencez secteur</translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>Nombre de secteurs</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>Nouvelle partition Taille:</translation>
    </message>
    <message>
        <source>bytes</source>
        <translation>octets</translation>
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
        <translation>Type de Partition</translation>
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
        <translation>Créer et initialiser un nouveau système de fichiers</translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** FREE **</translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>Pas d&apos;espace libre!</translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>Modifier les propriétés de pool</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Propriété</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>Taille totale du pool de stockage.</translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>Pourcentage d&apos;espace utilisé du pool.</translation>
    </message>
    <message>
        <source>Alternate root directory. If set, this directory is prepended to any
mount points within the pool. This can be used when examining an
unknown pool where the mount points cannot be trusted, or in an
alternate boot environment, where the typical paths are not valid.
altroot is not a persistent property. It is valid only while the
system is up.  Setting altroot defaults to using cachefile=none, though
this may be overridden using an explicit setting.</source>
        <translation>Répertoire racine secondaire. S&apos;il est défini, ce répertoire est ajouté en cas de
points de montage à l&apos;intérieur de pool. Ceci peut être utilisé lors de l&apos;examen d&apos;un
pool inconnu où les points de montage ne ​​sont pas fiables, ou dans un
environnement d&apos;initialisation de remplacement, où les chemins typiques ne sont pas valides.
altroot n&apos;est pas une propriété persistante. Elle est valable uniquement si le
système est en place. Réglage altroot utilise par défaut cachefile = aucune, si
cela peut être outrepassé en utilisant un cadre explicite.</translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation>L&apos;état ​​de santé actuel de pool.</translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>Un identifiant unique pour le pool. </translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation>La version sur le disque actuel de pool. Cela peut être augmentée, mais
jamais diminué. La méthode préférée de la mise à jour des pools consiste à le
commande &quot;zpool upgrade&quot;, bien que cette propriété peut être utilisée quand un
version spécifique est nécessaire pour la compatibilité ascendante. Cette propriété
peut être n&apos;importe quel nombre entre 1 et la version actuelle rapporté par
&quot;zpool upgrade-v&quot;.</translation>
    </message>
    <message>
        <source>Identifies the default bootable dataset for the root pool. This
property is expected to be set mainly by the installation and upgrade
programs.</source>
        <translation>Identifie le jeu de données d&apos;initialisation par défaut du pool racine. Cette \ npropriété devrait être mis principalement par l&apos;installation et la mise à niveau \ nprogrammes.</translation>
    </message>
    <message>
        <source>Controls whether a non-privileged user is granted access based on the
dataset permissions defined on the dataset. See zfs(8) for more
information on ZFS delegated administration.</source>
        <translation>Contrôle si un utilisateur non-privilégié est accordé à l&apos;accès basé sur les
permissions d&apos;ensembles de données définis sur l&apos;ensemble de données. Voir ZFS (8) pour plus d&apos;
informations sur l&apos;administration déléguée de ZFS.</translation>
    </message>
    <message>
        <source>Controls automatic device replacement. If set to &quot;off&quot;, device
replacement must be initiated by the administrator by using the
&quot;zpool replace&quot; command. If set to &quot;on&quot;, any new device, found in the
same physical location as a device that previously belonged to the
pool, is automatically formatted and replaced. The default behavior
is &quot;off&quot;.  This property can also be referred to by its shortened
column name, &quot;replace&quot;.</source>
        <translation>Contrôle le remplacement automatique d&apos;appareil. S&apos;il est réglé sur &quot;off&quot;, le périphérique
de remplacement doit être initié par l&apos;administrateur en utilisant le
&quot;zpool replace&quot; commande. S&apos;il est réglé sur &quot;on&quot;, tout nouveau périphérique se trouvant dans le répertoire
même emplacement physique qu&apos;un périphérique qui appartenait à ce
pool est automatiquement formaté et remplacé. Le comportement par défaut
est &quot;off&quot;. Cette propriété peut également être désigné par son raccourci
nom de colonne, &quot;remplacer&quot;.</translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation>Détermine si les informations sur les instantanés associés à ce
pool est émit lorsque  &quot;zfs list&quot; est exécuté sans l&apos;option-t. La
valeur par défaut est off.</translation>
    </message>
    <message>
        <source>Controls automatic pool expansion when the underlying LUN is grown.
If set to &quot;on&quot;, the pool will be resized according to the size of the
expanded device. If the device is part of a mirror or raidz then all
devices within that mirror/raidz group must be expanded before the
new space is made available to the pool. The default behavior is
&quot;off&quot;.  This property can also be referred to by its shortened column
name, expand.</source>
        <translation>Contrôle d&apos;extension de pool automatique lorsque le LUN sous-jacente est cultivé.
S&apos;il est réglé sur &quot;on&quot;, le pool sera redimensionné selon la taille de la 
dispositif élargi. Si l&apos;appareil fait partie d&apos;un miroir ou raidz puis tous
dispositifs à l&apos;intérieur de ce groupe miroir / raidz doivent être élargis avant le
nouvel espace est rendu disponible dans la piscine. Le comportement par défaut est
&quot;off&quot;. Cette propriété peut également être désigné par sa colonne raccourci
nommer, développez.</translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation>Seuil pour le nombre de blocs idem copies. Si la référence
comptent pour dédupliquées bloc augmente au-dessus de ce nombre, un nouveau
idem copie de ce bloc est automatiquement mémorisé. Le réglage par défaut est
0.</translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation>Le taux de duplication spécifié pour un pool, exprimé comme un multiplicateur. Par
exemple, une valeur de 1,76 indique que 1,76 unités de données ont été stockées, mais
seulement 1 unité d&apos;espace disque a été effectivement consommé. Voir ZFS (8) pour une description
de la fonction de déduplication.  </translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation>Nombre de blocs dans le pool qui ne sont pas affectés.</translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation>Quantité d&apos;espace de stockage dans le pool qui a été alloué physiquement.</translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation>Une chaine de commentaire arbitraire défini par l&apos;administrateur.</translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation>Cette propriété a actuellement aucune valeur sur FreeBSD</translation>
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
        <translation>Contrôles de l&apos;emplacement où la configuration de pool est mise en cache.
Découvrir tous les pools au démarrage du système nécessite une copie en cache de la
les données de configuration qui sont stockées sur le système de fichiers racine. Toutes les piscines
dans ce cache sont importés automatiquement au démarrage du système. Certains
environnements, tels que l&apos;installation et le clustering, doivent mettre en cache
informations dans un emplacement différent de sorte que les pools ne sont pas automatiquement
importés. Définition de cette propriété met en cache de la configuration du pool
à un autre endroit qui peut ensuite être importé avec &quot;zpool import
-c &quot;. Réglage à la valeur spéciale&quot; none &quot;crée un pool temporaire
qui n&apos;est jamais mis en cache, et la valeur spéciale&apos;&apos; (chaîne vide) utilise
l&apos;emplacement par défaut</translation>
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
        <translation>Si réglé sur, le pool sera importé en mode lecture seule avec le
restrictions suivantes:
* Données synchrones dans le journal d&apos;intention ne seront pas accessibles
* Propriétés des pools ne peuvent être modifiés
* Les ensembles de données de ce pool ne peuvent être montés en lecture seule
* Pour écrire en lecture seule ce pool, une exportation et l&apos;importation de ce pool
est nécessaire.
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
        <translation>Nouveau système de fichiers</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nom:</translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation>Propriétés du remplacement au moment de la création:</translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation>Empêcher le montage automatique (canmount = noauto)</translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>Point de montage</translation>
    </message>
    <message>
        <source>none</source>
        <translation>Aucun </translation>
    </message>
    <message>
        <source>legacy</source>
        <translation>héritage</translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[chemin]</translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>Force UTF-8 seulement (utf8only=on)</translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>Unicode normalization =</translation>
    </message>
    <message>
        <source>formD</source>
        <translation>formD</translation>
    </message>
    <message>
        <source>formKCF</source>
        <translation>formKCF</translation>
    </message>
    <message>
        <source>Copies =</source>
        <translation>Copies =</translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation>Dé duplication (dedup = on)</translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation>Compression =</translation>
    </message>
    <message>
        <source>off</source>
        <translation>off</translation>
    </message>
    <message>
        <source>on</source>
        <translation>on</translation>
    </message>
    <message>
        <source>lzjb</source>
        <translation>lzjb</translation>
    </message>
    <message>
        <source>zle</source>
        <translation>zle</translation>
    </message>
    <message>
        <source>gzip</source>
        <translation>gzip</translation>
    </message>
    <message>
        <source>gzip-1</source>
        <translation>gzip-1</translation>
    </message>
    <message>
        <source>gzip-2</source>
        <translation>gzip-2</translation>
    </message>
    <message>
        <source>gzip-3</source>
        <translation>gzip-3 </translation>
    </message>
    <message>
        <source>gzip-4</source>
        <translation>gzip-4</translation>
    </message>
    <message>
        <source>gzip-5</source>
        <translation>gzip-5</translation>
    </message>
    <message>
        <source>gzip-6</source>
        <translation>gzip-6</translation>
    </message>
    <message>
        <source>gzip-7</source>
        <translation>gzip-7</translation>
    </message>
    <message>
        <source>gzip-8</source>
        <translation>gzip-8</translation>
    </message>
    <message>
        <source>gzip-9</source>
        <translation>gzip-9</translation>
    </message>
</context>
<context>
    <name>ZManagerWindow</name>
    <message>
        <source>PCBSD Disk Manager</source>
        <translation>PCBSD Gestionnaire de disque</translation>
    </message>
    <message>
        <source>Disks</source>
        <translation>Disques</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Périphériques</translation>
    </message>
    <message>
        <source>State</source>
        <translation>État</translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation>ZFS Pools</translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation>Nom du Pool</translation>
    </message>
    <message>
        <source>Status:</source>
        <translation>Statut:</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation>TexLabel</translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation>Effacer l&apos;erreur</translation>
    </message>
    <message>
        <source>poolname</source>
        <translation>poolnom</translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation>ZFS Système de fichier</translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation>Système de fichier</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Statut</translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation>Raffraichir...</translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>Pas de piscines disponibles, faites un clic droit pour créer un nouveau...</translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation>(Pas média dans le lecteur)</translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation>Disponible</translation>
    </message>
    <message>
        <source>No disk</source>
        <translation>Pas de disque</translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation>ZPool:</translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation>Tranches</translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation>Monter:</translation>
    </message>
    <message>
        <source>Available</source>
        <translation>Disponible </translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation>Démonter</translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation>Partitionner</translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation>(Exportés)
</translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(Détruit)
</translation>
    </message>
    <message>
        <source>Offline</source>
        <translation>Hors ligne</translation>
    </message>
    <message>
        <source>Online</source>
        <translation>En Ligne </translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation>Dégradé</translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation>Défaillant</translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>Suppression</translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation>Indisponible</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Inconnue</translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation>Créer un nouveau pool</translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation>Renommer un Pool</translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation>Détruire un pool </translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation>Ajouter un périphérique</translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>Ajouter des périphériques de journal...</translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>Ajouter des périphériques de cache...</translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>Ajouter des périphériques de rechange...</translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>Scrub</translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>Exporter un pool</translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>Importer un pool</translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>Récupérer un pool détruit</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Propriétés...</translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>Fixez dispositif (miroir)...</translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation>Détacher du miroir</translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation>Mettez hors ligne</translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation>Mettez en ligne</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Retirer</translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation>zpool Menu</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Périphérique Menu</translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation>Démontez</translation>
    </message>
    <message>
        <source>Mount</source>
        <translation>Monter</translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation>Créer une table de partition MBR</translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>Créer une table de partition GPT</translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>Créer une table de partition BSD</translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation>Supprimer la table de partition</translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation>Ajouter une nouvelle tranche</translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation>Ajouter une nouvelle partition</translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation>Détruisez cette tranche</translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation>Détruisez cette partition</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Attention</translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation>Cette opération ne peut être annulée.
OK pour détruire la tranche / partition?</translation>
    </message>
    <message>
        <source>Error report</source>
        <translation>Rapport d&apos;erreur</translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation>Créer un nouveau zpool</translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>Importante information</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>Le pool a été importé en mode lecture seule, donc de tenter de détruire le pool va laisser le pool pool dans l&apos;état où il était lors de l&apos;importation, pas nécessairement détruite.</translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>Fixez les dispositifs de miroir à</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation>Le pool a été importé en mode lecture seule, donc de tenter d&apos;exporter le pool va laisser le pool dans l&apos;état où il était lors de l&apos;importation, pas nécessairement exportés.</translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>Récupérer le pool détruit en...</translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>Importer le pool comme...</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>Le pool a été importé en mode lecture seule, elle ne peut pas être renommé.</translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>Ajouter plus d&apos;appareils à zpool</translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>Ajouter des périphériques de journal à zpool</translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>Ajouter des périphériques de cache à zpool</translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>Ajouter des périphériques de rechange pour zpool</translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[Montage]</translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[Non Montage]  </translation>
    </message>
    <message>
        <source> of </source>
        <translation> de     </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation>Renommer la base de données</translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation>Créer une nouvelle base de données</translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation>Créer un jeu de données clonés</translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation>Détruisez ensemble de données</translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation>Promouvoir système de fichiers</translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation>Prendre un instantané</translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation>Détruisez un instantané</translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation>Restauration de cet instantané</translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation>Modifier les propriétés</translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation>zfs Menu</translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation>Confirmation</translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation>Prendre un nouvel instantané</translation>
    </message>
    <message>
        <source>New name</source>
        <translation>Nouveau nom</translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
        <translation>Cette opération ne peut être annulée et fera toutes les données ajoutées après l&apos;instantané à perdre.
Toutes les instantanés créés après celui-ci seront supprimés, ainsi que tous les systèmes de fichiers clone qui en dépendent.
</translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
Proceed with recovery?</source>
        <translation>Ce pool a été détruite, et ses disques peut avoir été réutilisés. Tenter de le récupérer va détruire de nouvelles données qui pourraient avoir été stockée dans les dispositifs qui ont été réutilisés et ne peuvent pas être récupérés.
Procéder au recouvrement?</translation>
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
