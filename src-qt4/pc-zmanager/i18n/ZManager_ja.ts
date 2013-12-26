<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>ダイアログ</translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>再帰的に全ての変更を適用する</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>プロパティ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>値</translation>
    </message>
    <message>
        <source>Source</source>
        <translation>ソース</translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>ユーザープロパティの追加</translation>
    </message>
    <message>
        <source>...</source>
        <translation>…</translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>ユーザープロパティの削除</translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>親から継承する</translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>変更を元に戻す</translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>ファイルとディレクトリが作成されるときにACLエントリが継承される方法を制御します。
値は、discard、noallow、restricted、passthroughです。これらの値の説明については、ACLプロパティを参照してください。</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>ACLエントリーは、chmodコマンドを動作中に変更された方法を制御します。
値は破棄、groupmask、およびpassthroughです。
これらの値の説明については、ACLプロパティを参照してください。</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>ファイルを読み取るときにファイルのアクセス時間が更新されるかどうかを制御します。
それはメーラーのようなユーティリティーを混乱させる可能性がありますが、
このプロパティをオフにすると、ファイルを読み込む際に書き込みトラフィックを発生が回避され、
パフォーマンスが大幅に向上する可能性があります。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation type="unfinished">
            </translation>
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>データの整合性を検証するために使用されるチェックサムを制御します。
デフォルト値は、自動的に適切なアルゴリズムを選択するがonになっています。
(現在は、fletcher4をしていますが、今後のリリースで変更される可能性があります)
値をoffにするとユーザーデータの整合性チェックが無効になります。
チェックサムを無効にするのは推奨されません。</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>有効またはデータセットの圧縮を無効にします。
値は、on、off、lzjb、zle、gzipとgzip-Nです。
現在、このプロパティにlzjb、gzip、あるいはgzip-Nを設定することは、このプロパティをonに設定するのと同じ効果があります。
既存のデータを持つファイルシステムで圧縮を有効にすると、新しいデータのみを圧縮します。既存のデータは圧縮されていないままです。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation>ファイルシステムごとにユーザーデータのコピー数を設定します。
これらのコピーは、任意のプールレベルの冗長性を増やします。
ユーザーデータの複数のコピーが使用するディスク容量は、
対応するファイルとデータセットに蓄積され、クォータと予約に対してカウントされます。
複数のコピーが有効になっているときだけでなく、使用されるプロパティが更新されます。
既存のファイルシステム上では、このプロパティを変更すると、新しく書き込まれたデータのみに影響するため、
ファイルシステムの作成時に、このプロパティを設定することを考慮してください。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>読み取り専用のデータセットが作成された日付と時刻を特定するプロパティ。</translation>
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
        <translation>データセットの重複排除を構成します。
デフォルト値はオフになっています。
デフォルトの重複排除チェックサムはSHA256です。
(これは将来変更される場合があります)  
dedupを有効にするときに定義されたチェックサムは、既に設定されているチェックサムプロパティを上書きします。
検証する値を設定すると、SHA256で確認する設定と同じ効果があります。
検証するために設定した場合、ZFSはブロックの内容が同一であることを確認するために
同じシグネチャを持つ2つのブロックの場合はbyte-to-byteの比較を行います。</translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>デバイスプロパティは現在FreeBSDではサポートされていません。</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>プロセスがこのファイルシステム内から実行できるかどうかを制御します。
デフォルト値はONです。</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>読み取り専用のファイルシステム、クローン、またはスナップショットがマウントされているかどうかを示すプロパティ。</translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation>このファイルシステムに使用されるマウントポイントを制御します。
マウントポイントのプロパティがファイルシステムに変更がかかるのはファイルシステムに属しているマウントポイントが全てアンマウントされたときです。
もし、新しい値がレガシーの場合はアンマウントされたままになります。
それ以外の場合、もし、プロパティが以前はレガシー、またはnoneだった場合や、
以前のマウント先のプロパティが変更された場合は自動的に新しい場所に再マウントされます。
また、任意の共有ファイルシステムは、共有が解除されて、新しい場所で共有されます。</translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation>一次キャッシュ(ARC)にキャッシュされる内容を制御します。
可能な値は、すべて、なし、およびメタデータです。
すべてに設定した場合、ユーザーデータとメタデータの両方がキャッシュされます。
noneに設定すると、ユーザーデータやメタデータのいずれもがキャッシュされます。メタデータに設定すると、メタデータのみがキャッシュされます。</translation>
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
        <translation>データセットを変更できるかどうかを制御します。
onに設定すると変更を行うことはできません。</translation>
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
        <translation>データセットが消費できるディスク容量を設定します。
このプロパティは使用されている領域の量に強い制限が適用されます。
この強い制限値には、スナップショットやクローンなどの下位データで使用されるディスクスペースは含まれていません。</translation>
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
        <translation>スナップショットやクローンなどの子孫は含まないデータセットに保証されているディスクスペースの最小量を設定します。
使用されるディスク領域の量がこの値を下回る場合は
refreservationをに指定された容量を占有しているかのようにデータセットが処理されます。
refreservationの予約は、親データセットの割り当て制限と
予約に対する親データセットのディスク使用領域およびカウントに計上されます。
refreservationが設定されている場合、
スナップショットは十分な空きプール領域が
データセット内の参照予約されている
現在のバイト数を除いて利用可能な場合のみ許可されています。</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space guaranteed to a dataset and its descendents.
When the amount of disk space used is below this value, the dataset is treated as if
it were using the amount of space specified by its reservation. Reservations are
accounted for in the parent dataset&apos;s disk space used, and count against the parent
dataset&apos;s quotas and reservations.</source>
        <translation>データセットとその子孫に対して保証のディスクスペースの最小量を設定します。
使用されるディスク領域の量がこの値を下回る場合、
その予約に指定された容量を使用していたかのようにデータセットは処理されます。
予約は使用する親データセットのディスクスペースで考慮し、
親データセットのクォータと予約にカウントされています。</translation>
    </message>
    <message>
        <source>Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,
and metadata. If set to all, both user data and metadata are cached. If set to none,
neither user data nor metadata is cached. If set to metadata, only metadata is cached.</source>
        <translation>二次キャッシュ（L2ARC）にキャッシュされる内容を制御します。
可能な値は、すべて、なし、およびメタデータです。すべてに設定した場合、ユーザーデータとメタデータの両方がキャッシュされます。
noneに設定すると、ユーザーデータやメタデータのいずれもがキャッシュされます。メタデータに設定すると、メタデータのみがキャッシュされます。</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>setuidビットがファイルシステムにおいて名誉あるかどうかを制御します。</translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>ファイルシステムがNFSを介して利用可能であり、
どのようなオプションを使用するかどうかを制御します。
ONに設定すると、zfs shareコマンドをオプションなしで起動されます。
それ以外の場合は、zfs shareコマンドは、このプロパティの内容と同等のオプションを指定して呼び出されます。</translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>sharesmbプロパティは現在FreeBSDでは何も影響はありません。</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>.zfsディレクトリはファイルシステムのルートを非表示にするか表示するかを制御します。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation type="unfinished">
            </translation>
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
        <translation>ボリュームの為にボリュームの論理サイズを指定します。</translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation>ボリュームの為にボリュームのブロックサイズを指定します。</translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation>データセットがjailに追加されているかどうかを示します。</translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>このプロパティはFreeBSDではサポートしていません。</translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>このファイルシステムはアンマウント後破壊される予定になっています。</translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation>このデータセットの空き領域を圧縮するための圧縮率は
乗数として表現されます。</translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation>前回のスナップショット以降に
このデータセットに書き込まれた参照領域の量。</translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation>このプロパティは、ユーザーの数に設定され、このスナップショットに保持しています。
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
        <translation>このプロパティはシステムが作成した後は変更できません。</translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>ユーザープロパティ</translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>不明なプロパティか、情報が存在しません。
詳しくはZFSのマニュアルを読んでください。</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation>[無効]</translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation>[継承]</translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation>[追加済み]</translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation>[変更済み]</translation>
    </message>
    <message>
        <source>User property</source>
        <translation>ユーザープロパティ</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>デバイスメニュー</translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation>ユーザープロパティを追加する</translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation>ユーザープロパティを削除する</translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation>ダイアログ</translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation>マウントするデバイス:</translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation>(デバイス)</translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation>マウントする場所:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>…</translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation>ディレクトリーを選択</translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation>ダイアログ</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>プール名:</translation>
    </message>
    <message>
        <source>Read only</source>
        <translation>読み込みのみ</translation>
    </message>
    <message>
        <source>Change altroot:</source>
        <translation>altrootを変更する:</translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation>ダイアログ</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>選択済み</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>プール名:</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>設定タイプ:</translation>
    </message>
    <message>
        <source>Striped</source>
        <translation>ストライプ</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>ミラー</translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation>Raid-Z (シングルパリティ)</translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>Raid-Z2 (ダブルパリティ)</translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>Raid-Z3 (トリプルパリティ)</translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>キャッシュ</translation>
    </message>
    <message>
        <source>Log</source>
        <translation>ログ</translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>予備</translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>エラーの説明はここです</translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation>有効なプール名を提供しなくてはいけません</translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>正確に %1 デバイスを選択しなくてはいけません</translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>最低一つはデバイスを選択しなくてはいけません</translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>パーティションを追加する</translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation>新しいパーティションもしくはスライスを追加する</translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 ギガバイト</translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>デバイス:</translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>ada0</translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>利用できる最大の空きブロック:</translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>合計サイズ:</translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation>現在のレイアウト:</translation>
    </message>
    <message>
        <source>Designation</source>
        <translation>指定</translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>開始セクター</translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>セクター数</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>サイズ</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>色</translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>新しいパーティションのサイズ:</translation>
    </message>
    <message>
        <source>bytes</source>
        <translation>バイト</translation>
    </message>
    <message>
        <source>kB</source>
        <translation>キロバイト</translation>
    </message>
    <message>
        <source>MB</source>
        <translation>メガバイト</translation>
    </message>
    <message>
        <source>GB</source>
        <translation>ギガバイト</translation>
    </message>
    <message>
        <source>TB</source>
        <translation>テラバイト</translation>
    </message>
    <message>
        <source>Partition type:</source>
        <translation>パーティションの種類:</translation>
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
        <translation>新しいファイルシステムを作成して初期化する</translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** 空き **</translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>空き領域はありません!</translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>プールのプロパティを編集する</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>プロパティ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>値</translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>ストレージプールの合計サイズ。</translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>プールの使用率。</translation>
    </message>
    <message>
        <source>Alternate root directory. If set, this directory is prepended to any
mount points within the pool. This can be used when examining an
unknown pool where the mount points cannot be trusted, or in an
alternate boot environment, where the typical paths are not valid.
altroot is not a persistent property. It is valid only while the
system is up.  Setting altroot defaults to using cachefile=none, though
this may be overridden using an explicit setting.</source>
        <translation>代替ルートディレクトリー。
設定した場合、このディレクトリーはプール内の任意のマウントポイントに追加されます。
どこにマウントされるか信頼できないプールや、典型的なパスが有効でない代替ブート環境を検査するときに使用することができます。
altrootは永続プロパティではありません。
それはシステムが稼働している間のみ有効です。  
altrootを設定するとデフォルトでcachefile=noneが設定されますが、
明示的に設定を使用して上書きした方がいいでしょう。</translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation>プールの現在の健康状態。</translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>プールのユニークな識別子。</translation>
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
        <translation>ルートプールのデフォルトのブート可能データセットを識別します。
このプロパティは、インストールおよびアップグレードプログラムを中心に
設定されることが期待されています。</translation>
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
        <source>Controls the location of where the pool configuration is cached. Disâ
covering all pools on system startup requires a cached copy of the
configuration data that is stored on the root file system. All pools
in this cache are automatically imported when the system boots. Some
environments, such as install and clustering, need to cache this
information in a different location so that pools are not automatiâ
cally imported. Setting this property caches the pool configuration
in a different location that can later be imported with &quot;zpool import
-c&quot;.  Setting it to the special value &quot;none&quot; creates a temporary pool
that is never cached, and the special value &apos;&apos; (empty string) uses
the default location.</source>
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
        <source>If set to on, pool will be imported in read-only mode with the folâ
lowing restrictions:
  Â·   Synchronous data in the intent log will not be accessible
  Â·   Properties of the pool can not be changed
  Â·   Datasets of this pool can only be mounted read-only
  Â·   To write to a read-only pool, a export and import of the pool
      is required.
</source>
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
        <translation>新しいファイルシステム</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名前:</translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation>作成時に上書きするプロパティ:</translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation>自動マウントを阻止する (canmount=noauto)</translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>マウントポイント</translation>
    </message>
    <message>
        <source>none</source>
        <translation>無し</translation>
    </message>
    <message>
        <source>legacy</source>
        <translation type="unfinished">legacy</translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[パス]</translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>UTF-8のみを強制する (utf8only=on)</translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>Unicodeの正規化 =</translation>
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
        <translation>コピー =</translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation>重複排除 (dedup=on)</translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation>圧縮 =</translation>
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
        <translation>PC-BSDディスクマネージャー</translation>
    </message>
    <message>
        <source>Disks</source>
        <translation>ディスク</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <source>State</source>
        <translation>状態</translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation>ZFSプール</translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation>プール名</translation>
    </message>
    <message>
        <source>Status:</source>
        <translation>状態:</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation>ラベル</translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation>エラーを削除する</translation>
    </message>
    <message>
        <source>poolname</source>
        <translation>プール名</translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation>ZFSファイルシステム</translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation>ファイルシステム</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>状態</translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation>再読込中…</translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>プールは何もありませんので右クリックで新しく作成してください…</translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation> (ドライブにメディアが入っていません)</translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation>利用できます</translation>
    </message>
    <message>
        <source>No disk</source>
        <translation>ディスクがありません</translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation>ZPool: </translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation>スライス</translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation>マウントされている場所:</translation>
    </message>
    <message>
        <source>Available</source>
        <translation>利用できます</translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation>アンマウントされました</translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation>パーティションが作成されました</translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation type="unfinished">(出力されました)
</translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(破壊されました)
</translation>
    </message>
    <message>
        <source>Offline</source>
        <translation>オフライン</translation>
    </message>
    <message>
        <source>Online</source>
        <translation>オンライン</translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation>劣化しました</translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation>障害が発生しました</translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>除去しました</translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation>利用できません</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>不明</translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation>新しいプールを作成する</translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation>プールの名前を変更する</translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation>プールを破壊する</translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation>デバイスを追加する…</translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>ログデバイスを追加する…</translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>キャッシュデバイスを追加する…</translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>予備デバイスを追加する…</translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>スクラブ</translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>プールを書き出す</translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>プールを読み込む</translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>破壊したプールを復旧する</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>プロパティ…</translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>デバイス及びミラーデバイスを追加する…</translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation>ミラーから切り離す</translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation>オフラインにする</translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation>オンラインにする</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation>zpoolメニュー</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>デバイスメニュー</translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation>アンマウント</translation>
    </message>
    <message>
        <source>Mount</source>
        <translation>マウント</translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation>MBRパーティションテーブルを作成する</translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>GPTパーティションテーブルを作成する</translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>BSDパーティションテーブルを作成する</translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation>パーティションテーブルを削除する</translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation>新しいスライスを追加する</translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation>新しいパーティションを追加する</translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation>このスライスを破壊する</translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation>このパーティションを破壊する</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation>この操作は元に戻せません。
スライスもしくはパーティションを破壊してよろしいですか?</translation>
    </message>
    <message>
        <source>Error report</source>
        <translation>エラーレポート</translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation>新しいzpoolを作成する</translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>重要なお知らせ</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>このプールはリードオンリーでインポートされたので、この状態で破壊を試みても破壊されることは無いでしょう。</translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>ミラーデバイスを次へ追加する </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>破壊されたプールを回復…</translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>プールをインポート…</translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks my have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
OK to proceed with recovery?</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>このプールはリードオンリーでインポートされたので名前の変更はできません。</translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>zpoolへデバイスを追加する</translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>zpoolへログデバイスを追加する</translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>zpoolへキャッシュデバイスを追加する</translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>zpoolへ予備デバイスを追加する</translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[マウントされています]</translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[マウントされていません]</translation>
    </message>
    <message>
        <source> of </source>
        <translation> of </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation>データセット名を変更する</translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation>新しいデータセットを作成する</translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation>クローンデータセットを作成する</translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation>データセットを破壊する</translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation>ファイルシステムをプロモーションする</translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation>スナップショットを作成する</translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation>スナップショットを破壊する</translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation>このスナップショットを巻き戻す</translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation>プロパティを編集する</translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation>zfsメニュー</translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation>確認</translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation>新しいスナップショットを作成する</translation>
    </message>
    <message>
        <source>New name</source>
        <translation>新しい名前</translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
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
