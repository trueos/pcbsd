<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>Діалог</translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>Застосувати зміни рекурсивно до всіх нащадків</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Властивість</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Значення</translation>
    </message>
    <message>
        <source>Source</source>
        <translation>Джерело</translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>Додати власну властивість</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>Вилучити власну властивість</translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>Наслідувати</translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>Скасувати зміни</translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>Як успадковуються елементи ACL в разі створення файлів та директорій.
Можливі значення - discard, noallow, restricted, та passthrough. Опис цих значень дивіться
в ACL Properties.</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>Як змінюється елемент ACL під час операції chmod.
Можливі значення - discard, groupmask, та passthrough. Опис цих
значень дивіться в ACL Properties.</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>Зміна часу доступу до файлу під час його читання. Вимкнення цієї
властивості запобігає створенню трафіка на запис під час читання файлів,
що може значно підвищити продуктивність, хоча і може заплутати поштові
та аналогічні утиліти.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>Властивість тільки для читання, яка показує доступний для датасету та
всіх його нащадків обсяг дискового простору за умови відсутності іншої
активності в пулі. Через те що дисковий простір спільний в пулі, доступний
простір можуть обмежувати різні фактори, такі як фізичний розмір пулу,
квоти, резерви та інші датасети в пулі.</translation>
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
        <translation>Якщо ця властивість вимкнена, файлова система не може буде
змонтованою і вона буде ігнорована командою &apos;zfs mount -a&apos;.
Вимкнення цієї властивості аналогічне до встановлення значення
властивості точки монтування на &apos;не вказано&apos;, за винятком того, що
датасет все ще має нормальну точку монтування, яку можна успадкувати.
Вимкнення цієї властивості дозволяє використовувати датасети
виключно як механізм для успадкування властивостей. Наприклад,
встановлення canmount=off дозволяє мати два датасети з однією й тією
ж точкою монтування, внаслідок чого нащадки обох датасетів з&apos;являються
в одній і тій же директорії, але мають різні успадковані характеристики.</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>Застосування контрольних сум для перевірки цілісності даних.
Типове значення &apos;on&apos;, що автоматично обирає відповідний
алгоритм (наразі fletcher4, але це може змінитися в наступних
релізах). При значенні &apos;off&apos; перевірка цілісності даних
не проводитиметься. Зазвичай відключення перевірки
контрольних сум не рекомендується.</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>Стиснення датасету. Можливі значення - on, off, lzjb, zle, gzip, та gzip-N.
Наразі встановлення значення lzjb, gzip, або gzip-N має той же ефект,
що й значення &apos;on&apos;. Увімкнення стиснення на файловій системі з існуючими
даними матиме ефект тільки для нових даних. Існуючі дані
залишаться нестиснутими.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>Властивість тільки для читання, виражена як множник, яка показує ступінь
стиснення датасету. Стиснення можна увімкнути командою &apos;zfs set compression=on&apos;
відповідного датасету. Значення розраховується виходячи з логічного розміру
всіх файлів та кількості пов&apos;язаних фізичних даних. Воно показує чисту економію
за рахунок використання властивості стиснення.</translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>Властивість тільки для читання - дата та час створення датасету.</translation>
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
        <translation>Ця властивість пристроїв наразі не підтримується у FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>Можуть чи ні з файлів на цій файловій системі виконуватися
процеси. Типове значення - &apos;on&apos; (так).</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>Властивість (тільки для читання), яка показує, чи файлова система,
клон чи знімок наразі змонтовані.</translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation type="unfinished">Контролює точку монтування, яка використовується для цієї файлової системи.
Коли точка монтування змінюється, файлова система та всі нащадки точки
монтування відмонтовуються. Якщо нове значення успадковане, вони
залишаються відмонтованими. В іншому випадку вони автоматично
перемонтовуються в новому розміщенні, незалежно від того, чи властивість
була попередньо успадкована чи ні, або якщо вони були змонтовані перед
зміною властивості. На додаток, будь які розшарені файлові системи перестають
такими бути і знову розшарюються в новому розміщенні.</translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>Заборона модифікації датасету. Коли значення &apos;on&apos; (так),
ніяких змін робити не можна.</translation>
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Sets the amount of disk space that a dataset can consume. This property
enforces a hard limit on the amount of space used. This hard limit does
not include disk space used by descendents, such as snapshots and clones.</source>
        <translation type="unfinished">
            </translation>
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>Властивість sharesmb наразі не має ефекту у FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>Прихована чи видима .zfs директорія в корені файлової системи.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>Властивість (тільки для читання), яка показує тип файлової системи (файлова система чи клон), розділ або знімок.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>Властивість (тільки для читання), яка показує споживання дискового простору датасетом та всіма його нащадками.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>Властивість (тільки для читання), яка показує дисковий простір зайнятий
нащадками датасету, і який можна звільнити, знищивши всі нащадки датасету.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation type="unfinished">
            </translation>
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation type="unfinished">
            </translation>
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>Ця властивість не підтримується FreeBSD.</translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>Цю файлову систему помічено на знищення після відмонтування.</translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation type="unfinished">
            </translation>
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
        <translation type="unfinished">
            </translation>
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>Властивість користувача</translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>Невідома властивість або інформація відсутня.
Перегляньте керівництво по ZFS для детальної інформації.</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>User property</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Назва пулу:</translation>
    </message>
    <message>
        <source>Do not mount any filesystems</source>
        <translation>Не монтувати жодної файлової системи</translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation>Читати тільки пул</translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation>Параметри імпорту:</translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation>Монтувати з альтернативним коренем:</translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation>Монтувати всі файлові системи автоматично</translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation>Поведінка монтування:</translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation>При помилці форсувати операцію (використовувати обережно)</translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation>Діалог</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>Вибрано</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Назва пулу:</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>Тип конфігурації:</translation>
    </message>
    <message>
        <source>Striped</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Дзеркало</translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation>Raid-Z (одиночна парність)</translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>Raid-Z2 (подвійна парність)</translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>Raid-Z3 (потрійна парність)</translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>Кеш</translation>
    </message>
    <message>
        <source>Log</source>
        <translation>Лог</translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>Запас</translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>Опис помилки</translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation>Необхідно вказати правильну назву пулу</translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>Необхідно обрати точну кількість пристроїв - %1</translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>Необхідно обрати принаймні один пристрій</translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>Додати партицію</translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation>Додати новий слайс/партицію</translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 Гб</translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>Пристрій:</translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>ada0</translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>Найбільший доступний вільний блок:</translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>Загальний розмір:</translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation>Поточна схема:</translation>
    </message>
    <message>
        <source>Designation</source>
        <translation>Мітка</translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>Початковий сектор</translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>Кількість секторів</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Розмір</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Колір</translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>Новий розмір партиції:</translation>
    </message>
    <message>
        <source>bytes</source>
        <translation>байтів</translation>
    </message>
    <message>
        <source>kB</source>
        <translation>Кб</translation>
    </message>
    <message>
        <source>MB</source>
        <translation>Мб</translation>
    </message>
    <message>
        <source>GB</source>
        <translation>Гб</translation>
    </message>
    <message>
        <source>TB</source>
        <translation>Тб</translation>
    </message>
    <message>
        <source>Partition type:</source>
        <translation>Тип партиції:</translation>
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
        <translation>Створити та ініціалізувати нову файлову систему</translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** ВІЛЬНО **</translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>Немає вільного місця!</translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>Редагувати властивості пулу</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Властивість</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Значення</translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>Загальний розмір пулу зберігання.</translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>Відсоток використання пулу.</translation>
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
        <translation>Поточний стан пулу.</translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>Унікальний ідентифікатор пулу.</translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation type="unfinished">
            </translation>
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
        <translation>Кількість невиділених блоків пулу.</translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation>Обсяг фізично виділеного дискового простору пулу.</translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation>Довільний рядок коментаря, встановлений адміністратором.</translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation>Ця властивість наразі не має значення у FreeBSD.</translation>
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
        <translation>Нова файлова система</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Назва:</translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation>Властивості, перевизначені в момент створення:</translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation>Без автомонтування (canmount=noauto)</translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>Точка монтування</translation>
    </message>
    <message>
        <source>none</source>
        <translation>не вказано</translation>
    </message>
    <message>
        <source>legacy</source>
        <translation>Успадковано</translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[шлях]</translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>Форсувати UTF-8 (utf8only=on)</translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>Юнікод нормалізація =</translation>
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
        <translation>Копій =</translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation>Дедуплікація (dedup=on)</translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation>Стиснення =</translation>
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
        <translation>gzip-3</translation>
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
        <translation>Керування дисками PCBSD</translation>
    </message>
    <message>
        <source>Disks</source>
        <translation>Диски</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Пристрій</translation>
    </message>
    <message>
        <source>State</source>
        <translation>Стан</translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation>Пули ZFS</translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation>Назва пулу</translation>
    </message>
    <message>
        <source>Status:</source>
        <translation>Статус:</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation>TextLabel</translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>poolname</source>
        <translation>poolname</translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation>Файлові системи ZFS</translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation>Файлові системи</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation>Оновлення...</translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>Немає жодного пулу, правий клік для створення нового...</translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation>Доступно</translation>
    </message>
    <message>
        <source>No disk</source>
        <translation>Немає диску</translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation>Z-пул: </translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation type="unfinished">Створено слайси</translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation>Змонтовано: </translation>
    </message>
    <message>
        <source>Available</source>
        <translation>Доступно</translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation>Відмонтовано</translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation type="unfinished">Розбито на партиції</translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation>(Експортовано)
</translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(Знищено)
</translation>
    </message>
    <message>
        <source>Offline</source>
        <translation>Офлайн</translation>
    </message>
    <message>
        <source>Online</source>
        <translation>Онлайн</translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation type="unfinished">Деградація</translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation type="unfinished">Збійний</translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>Видалено</translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation>Недоступно</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Невідомо</translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation>Створити новий пул</translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation>Змінити назву пулу</translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation>Знищити пул</translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation>Додати пристрій...</translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>Додати лог-пристрій...</translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>Додати кеш-пристрій...</translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>Додати запасний пристрій...</translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>Чистка</translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>Експортувати пул</translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>Імпортувати пул</translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>Відновити знищений пул</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Властивості...</translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>Приєднати (дзеркальний) пристрій...</translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation>Від&apos;єднати від дзеркала</translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation>В офлайн</translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation>В онлайн</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Видалити</translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation>Меню Z-пулу</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Меню пристрою</translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation>Відмонтувати</translation>
    </message>
    <message>
        <source>Mount</source>
        <translation>Змонтувати</translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation>Створити таблицю розділів MBR</translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>Створити таблицю розділів GPT</translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>Створити таблицю розділів BSD</translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation>Видалити таблицю розділів</translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation>Додати новий слайс</translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation>Додати нову партицію</translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation>Знищити цей слайс</translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation>Знищити цю партицію</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Попередження</translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation>Це безповоротна операція.
Згодні на знищення слайсу/партиції?</translation>
    </message>
    <message>
        <source>Error report</source>
        <translation>Звіт про помилки</translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation>Створити новий Z-пул</translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>Важлива інформація</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>Пул було імпортовано в режимі тільки для читання, тому спроба його знищення поверне пул в стан, який був під час імпорту, не обов&apos;язково знищений.</translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>Приєднати дзеркальний пристрій до </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation>Пул було імпортовано в режимі тільки для читання, тому спроба його експортувати поверне пул в стан, який був під час імпорту, не обов&apos;язково експортований.</translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>Відновити знищений пул як...</translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>Імпортувати пул як...</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>Пул було імпортовано в режимі тільки для читання, тому його не можна перейменувати.</translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>Додати пристрій до Z-пулу</translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>Додати лог-пристрій до Z-пулу</translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>Додати кеш-пристрій до Z-пулу</translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>Додати запасний пристрій до Z-пулу</translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[Змонтовано]</translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[Не змонтовано]</translation>
    </message>
    <message>
        <source> of </source>
        <translation> з </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation>Перейменувати датасет</translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation>Створити новий датасет</translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation>Клонувати датасет</translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation>Знищити датасет</translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation>Зробити знімок</translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation>Знищити знімок</translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation>Відкотитися до знімку</translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation>Редагувати властивості</translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation>zfs меню</translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation>Підтвердження</translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation>Зробити новий знімок</translation>
    </message>
    <message>
        <source>New name</source>
        <translation>Нова назва</translation>
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
