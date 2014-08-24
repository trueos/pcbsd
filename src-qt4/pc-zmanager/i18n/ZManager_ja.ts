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
        <translation>...</translation>
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
        <translation>ファイルとディレクトリーが作成されるときに ACL エントリーが継承される方法を制御します。
値は、discard、noallow、restricted、および passthrough です。これらの値の説明については、ACL プロパティを参照してください。</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>ACLエントリーは、chmod コマンドを動作中に変更された方法を制御します。
値は discard、groupmask、および passthrough です。
これらの値の説明については、ACL プロパティを参照してください。</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>ファイルを読み取るときにファイルのアクセス時間が更新されるかどうかを制御します。
このプロパティをオフにすると、メーラーのようなユーティリティーを混乱させる可能性
もありますが、ファイルを読み込む際に書き込みトラフィックの発生が回避され、
パフォーマンスが大幅に向上する可能性があります。</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>データの整合性を検証するために使用されるチェックサムを制御します。
デフォルト値は、自動的に適切なアルゴリズムを選択するが on になっています。
(現在は、fletcher4 が選択されますが、今後のリリースで変更される可能性があります)
値を off にするとユーザーデータの整合性チェックが無効になります。
チェックサムを無効にするのは推奨されません。</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>データセットの圧縮の有無を設定します。値は、on、off、lzjb、zle、gzip と
gzip-N です。現在、このプロパティに lzjb、gzip、あるいは gzip-N を設定すると、
このプロパティに on を設定することと同じ効果があります。既存のデータを持つ
ファイルシステムで圧縮を有効にすると、新しいデータのみを圧縮します。既存のデータは圧縮されないままです。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>この読み取り専用プロパティはデータセットの圧縮率をその乗数で示します。
圧縮はデータセットコマンド zfs set compression=on で有効になります。
この値は全ファイルの論理的サイズと物理データの総量によって計算されます。
計算結果には圧縮機能を使用して圧縮されたデータ含まれます。
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
        <translation>データセットが作成された日付と時刻を特定する読み取り専用のプロパティ。</translation>
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
        <translation>データセットの重複除去を構成します。デフォルト値は off です。
デフォルトの重複除去チェックサムは sha256 です。(これは将来
変更される場合があります)  重複除去を有効にするとチェックサムは、
既に定義されている checksum プロパティを上書きします。verify に設定すると
sha256,verify と同じ効果があります。
verify に設定した場合、2つのブロックが同じシグネチャを持つ場合には
ブロックの内容が同一であることを確認するために ZFS は1バイトずつの
比較を行います。</translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>現在 FreeBSD では devices プロパティはサポートされていません。</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>ファイルシステム、クローン、またはスナップショットが
マウントされているかどうかを示す読み取り専用のプロパティ。</translation>
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
それ以外の場合、もし、プロパティが以前はレガシー、または none だった場合や、
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
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation>データセットとその子孫が消費できるディスク容量を制限します。
このプロパティは、ファイルシステムやスナップショットなどの子孫によって
消費すべての領域を含む、使用されるディスク容量に固定の制限値を強制します。
既にクォータは、データセットの子孫でクォータを設定すると、
先祖のクォータを上書きではなく、追加的な制限を課しません。</translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>データセットを変更できるかどうかを制御します。
on に設定すると変更を行うことはできません。</translation>
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
        <translation>二次キャッシュ(L2ARC)にキャッシュされる内容を制御します。
可能な値は、すべて、なし、およびメタデータです。すべてに設定した場合、ユーザーデータとメタデータの両方がキャッシュされます。
none に設定すると、ユーザーデータやメタデータのいずれもがキャッシュされます。メタデータに設定すると、メタデータのみがキャッシュされます。</translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>現在 FreeBSD では sharesmb プロパティは無効です。</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>.zfs ディレクトリーはファイルシステムのルートを非表示にするか表示するかを制御します。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>ファイルシステムとしてのデータセットタイプの種類(ファイルシステムまたはクローン)、ボリューム、またはスナップショットを示す読み取り専用のプロパティです。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation>refreservation プロパティがデータセットにセットされている場合に、
refreservation を削除すると解放されるディスク領域の量を示す
読み取り専用のプロパティです。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation>データセットのスナップショットによって消費される
ディスク領域の量を特定する読み取り専用のプロパティです。
特にこのデータセットのスナップショットのすべてが破壊された場合に
解放されることになるディスク領域の量です。
領域は複数のスナップショットで共有できるので、この値は単純にスナップショットの合計ではないことに注意してください。</translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation>プールのバージョンとは独立したファイルシステムのディスク上のバージョンを識別します。
このプロパティはサポートされるソフトウェアのリリースから提供されて
以降のバージョンに設定することができます。</translation>
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
        <translation>データセットが jail に追加されているかどうかを示します。</translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>このプロパティは FreeBSD ではサポートしていません。</translation>
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
        <translation>前回のスナップショット以降にこのデータセットに書き込まれた
参照領域の量。</translation>
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
        <translation>このデータセットの同期リクエストの取り扱いについて ZFS へのヒントを
提供します。  logbias が latency(デフォルト)に設定されている場合、
ZFS は低遅延で要求を処理するために、プールログデバイスを使用します(設定済みの場合)。
logbias が throughput に設定されている場合、ZFS は設定されたプールの
ログデバイスを使用することはありません。  ZFS は代わりにグローバルプールの
スループットと資源の有効利用のために同期リクエストを最適化します。</translation>
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
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>データセットとその子が利用可能なディスク容量を示す読み取り専用のプロパティです。
ただし、他に書き込みが行われない場合に限ります。それはディスクスペースが
プール内で共有され、利用可能なディスク容量は物理的なディスク容量、プールの容量、
クオータ、データ使用量そしてプール内の他のデータセットの制約を受けるためです。</translation>
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
        <translation>このプロパティが off に設定されると、このファイルシステムは
マウントできなくなり、&apos;zfs mount -a&apos; でも無視されます。　この
プロパティを off に設定するのは mountpoint プロパティが
設定されたままであることを除けば mountpoint プロパティを
none に設定することと同じになります。このプロパティを off に
設定することは、プロパティの値を継承するために使えます。一例
として canmount=off を同一のマウントポイントを使用する
2つのデータセットに設定すると、それらの子孫は同じディレクトリーに
登場しますが、別々の特徴を継承できるようになります。</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>このファイルシステムに含まれるプログラムのプロセスを
起動できるかを制御します。デフォルト値は on です。</translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>ファイルシステムが NFS で利用可能か否か、どのようなオプションを
使用するかを制御します。on に設定すると、zfs share コマンドがオプション無しで
起動されます。それ以外の場合には、zfs share コマンドはこのプロパティの内容と
同等のオプションを指定して呼び出されます。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>データセットとその子孫が消費したディスクスペースの総量を示す読み取り専用のプロパティです。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>データセットの子が消費したディスクスペースの総量を示す
読み取り専用のプロパティです。
すべての子データセットが破棄されると解放されます。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation>データセット自身が消費したディスクスペースの総量を示す
読み取り専用のプロパティです。データセットとすべての
スナップショットと refreservation 予約が破棄されると解放されます。</translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation>クローンされたファイルシステムやボリュームがどのスナップショットから
クローンされたかを示す読み取り専用のプロパティです。クローンが存在する限り、
クローン元は破棄できません(-r や -f オプションを付けても)。
クローンされていないファイルシステムでは origin の値は none となります。</translation>
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
        <translation>ファイルシステムの推奨ブロックサイズを指定します。この
プロパティは固定サイズレコードでファイルにアクセスする
データベース処理のために作られたものです。  ZFS は内部の
アルゴリズムによって一般的なアクセスパターンに合わせて
自動的にブロックサイズを調整します。
一般的なファイルシステムの用途でこのプロパティを使用することは
性能に悪影響を及ぼす可能性があるため、特に推奨されません。
サイズの値は 512以上128Kバイト以下の2の累乗の値で
設定しなければなりません。</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation>データセットがアクセス可能なデータの総量を示す読み取り専用の
プロパティです。これはプール内に存在する他のデータセットと
共有されることがあります。</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>ファイルシステム内の setuid ビットの有効/無効を制御します。</translation>
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
        <translation>同期リクエスト (例:  fsync(2), O_DSYNC) の動作を制御します。
このプロパティは以下の値を受け付けます:
standard  これは POSIX の規定する動作で、全ての同期リクエスト
          が固定ストレージにデータを書き込まれたことを確認すると、
          全デバイスをフラッシュしてデバイスコントローラーに
          キャッシュされないようにします(これはデフォルトです)。
always    全てのファイルシステムトランザクションはシステムコールが
          値を返す前に書き込みとフラッシュを行います。これは
          パフォーマンス上不利になります。
disabled  同期リクエストを無効化します。ファイルシステムトランザクションは
          固定ストレージに対して常に周期的に書き込みを行うようになります。
          このオプションはパフォーマンスが最も高くなります。  しかし、
          ZFS がデータベース、NFSなどのアプリケーションからの
          同期トランザクション要求を無視するようになるため、
          とても危険です。  管理者はリスクを理解している場合のみ
          このオプションを使用して下さい。</translation>
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
        <translation>マウント位置:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
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
        <source>Do not mount any filesystems</source>
        <translation>ファイルシステムをマウントしないで下さい</translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation>プールのみを読み込む</translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation>インポートオプション:</translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation>代替のルートでマウントする:</translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation>全てのファイルシステムを自動マウントする</translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation>マウントの挙動:</translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation>エラーが起きた場合は強制に行う (気をつけて使用して下さい)</translation>
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
        <source>Must select at least one device</source>
        <translation>最低一つはデバイスを選択しなくてはいけません</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>構成タイプ:</translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>必ず %1 台のデバイスを選択してください</translation>
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
        <translation>代替ルートディレクトリーです。これが設定された場合、ディレクトリー名がプール内の
マウントポイントに追加されます。どこにマウントされるか信頼できないプールや、
典型的なパスが有効でない代替ブート環境を検査するときに使用することができます。
altroot は永続プロパティではありません。
それはシステムが稼働している間のみ有効です。  
altroot を設定するとデフォルトで cachefile=none が設定されますが、
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
        <translation>ディスク上の現在のプールのバージョンです。この値は増加させることは出来ますが、
減少させることは出来ません。下位互換性のために特定のバージョンを指定する必要が
ある場合には、このプロパティで設定することができますが、プールのアップデートには
&quot;zpool upgrade&quot; コマンドを使用する方がより望ましいです。このプロパティには
1から &quot;zpool upgrade -v&quot; で表示される最新バージョンまでのどの番号でも
指定できます。</translation>
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
        <translation>非特権ユーザーがデータセットで定義されたデータセットの権限に基づいて
アクセスを許可されるかどうかを制御します。
ZFS の委任管理の詳細については、zfs(8) を参照してください。</translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation>&quot;zfs list&quot; が -t オプションなしで実行したときにこのプールに関連付けられている
スナップショットの情報が出力されているかどうかを制御します。
デフォルト値は off です。</translation>
    </message>
    <message>
        <source>Controls automatic pool expansion when the underlying LUN is grown.
If set to &quot;on&quot;, the pool will be resized according to the size of the
expanded device. If the device is part of a mirror or raidz then all
devices within that mirror/raidz group must be expanded before the
new space is made available to the pool. The default behavior is
&quot;off&quot;.  This property can also be referred to by its shortened column
name, expand.</source>
        <translation>基盤となる LUN が増大する場合に行われる自動プール拡張を制御します。
&quot;on&quot; に設定すると、プールは拡張する装置の大きさに応じて変更されます。
デバイスがミラーや raidz の一部である場合、新しいスペースがプールで
利用できるようになるためには、そのミラー/raidz グループ内の
すべてのデバイスが拡張されている必要があります。
デフォルトの動作は &quot;off&quot; です。  このプロパティは短縮名 expand も使用できます。
</translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation>プールに指定の重複除去比は乗数で表されます。
たとえば、1.76 という値は、データの 1.76 単位が保存されましたが、
1 単位のディスクスペースだけが実際に消費されたことを示しています。
重複排除機能の詳細については、zfs(8) を参照してください。</translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation>割り当てられていないプール内のブロック数。</translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation>物理的に割り当てられているプール内のストレージスペースの量。</translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation>管理者が設定した任意のコメント文字列。</translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation>このプロパティは現在のFreeBSDには値がありません。</translation>
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
        <translation>プールの設定がキャッシュされている場所の位置を制御します。
システム起動時にすべてのプールを発見すると、ルートファイルシステムに
保存されているコンフィギュレーションデータのキャッシュコピーが必要になります。
システムのブート時にこのキャッシュ内のすべてのプールが自動的に
インポートされます。
このようなインストールやクラスタリングなどの一部の環境では、
プールが自動的にインポートされないように、別の場所にこの情報をキャッシュする必要があります。
このプロパティを設定すると、後で &quot;zpool import -c&quot; を
使用してインポートすることができる別の場所にあるプール構成を
キャッシュします。  特殊な値 &quot;none&quot; に設定すると、キャッシュされません
一時的なプールを作成し、特別な値 &apos;&apos;(空文字列) がデフォルトの場所を使用しています。</translation>
    </message>
    <message>
        <source>Controls automatic device replacement. If set to &quot;off&quot;, device
replacement must be initiated by the administrator by using the
&quot;zpool replace&quot; command. If set to &quot;on&quot;, any new device, found in the
same physical location as a device that previously belonged to the
pool, is automatically formatted and replaced. The default behavior
is &quot;off&quot;.  This property can also be referred to by its shortened
column name, &quot;replace&quot;.</source>
        <translation>自動デバイス交換を制御します。
&quot;off&quot; に設定すると、デバイスの交換は管理者が&quot;zpool replace&quot; コマンドで行います。
&quot;on&quot; に設定すると、プールに以前属していたデバイスと
物理的に同じ場所で検出された新しいデバイスは自動的に
フォーマットして交換されます。デフォルトの動作は &quot;off&quot; です。  
このプロパティは短縮名 &quot;replace&quot; も使用できます。</translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation>ブロック ditto コピー数の閾値です。重複除去されたブロックの
参照カウント数がこの値より増加すると、このブロックの
新しい ditto コピーがストアされます。
デフォルト設定値は0です。</translation>
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
        <translation>on に設定すると、プールが読み取り専用モードでインポートされます。
ただし以下の制限があります:
  *   インテントログ内の同期データにはアクセスできません
  *   プールのプロパティを変更することはできません
  *   このプールのデータセットは、読み取り専用でマウントできます
  *   読み取り専用のプールに書き込むには、プールのエクスポートと
      インポートが必要です。
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
        <translation>自動マウントしない (canmount=noauto)</translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>マウントポイント</translation>
    </message>
    <message>
        <source>none</source>
        <translation>none</translation>
    </message>
    <message>
        <source>legacy</source>
        <translation>legacy</translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[パス]</translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>UTF-8 のみ使用可 (utf8only=on)</translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>Unicode の正規化 =</translation>
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
        <translation>重複除去 (dedup=on)</translation>
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
        <translation>PC-BSD ディスクマネージャー</translation>
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
        <translation>ZFS プール</translation>
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
        <translation>ZFS ファイルシステム</translation>
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
        <translation>再読込中...</translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>利用可能なプールが無いので、右クリックで新しく作成してください...</translation>
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
        <translation>マウントされている場所: </translation>
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
        <translation>デバイスを追加する...</translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>ログデバイスを追加する...</translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>キャッシュデバイスを追加する...</translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>予備デバイスを追加する...</translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>スクラブ</translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>プールをエクスポートする</translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>プールをインポートする</translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>破壊したプールを復旧する</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>プロパティ...</translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>デバイス(ミラーデバイス)を追加する...</translation>
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
        <translation>zpool メニュー</translation>
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
        <translation>MBR パーティションテーブルを作成する</translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>GPT パーティションテーブルを作成する</translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>BSD パーティションテーブルを作成する</translation>
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
        <translation>新しい zpool を作成する</translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>重要なお知らせ</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>このプールは読み取り専用モードでインポートされたので、この状態でプールの破壊を試みても破壊されることは無いでしょう。</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation>このプールは読み取り専用モードでインポートされたので、この状態でプールのエクスポートを試みてもインポートされたままの状態になるため、エクスポートされないでしょう。</translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>破壊されたプールを回復...</translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>プールをインポート...</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>このプールはリードオンリーでインポートされたので名前の変更はできません。</translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>zpool へデバイスを追加する</translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>zpool へログデバイスを追加する</translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>zpool へキャッシュデバイスを追加する</translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>zpool へ予備デバイスを追加する</translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[マウント済み]</translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[マウントされていません]</translation>
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
        <translation>ZFS メニュー</translation>
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
        <translation>この操作ではスナップショットが作成された後に追加されたデータが消えるため元に戻すことはできません。
この後に作成されたスナップショットに加えて、依存するクローンファイルシステムも削除されます。
</translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(破棄されました)
</translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>削除済み</translation>
    </message>
    <message>
        <source> of </source>
        <translation> / </translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>ミラーデバイスの追加 </translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation>(エクスポート済)
</translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
Proceed with recovery?</source>
        <translation>プールは既に破壊されていますが、ディスクが再利用されている場合があります。回復を行うと機器が再利用されていた場合に書き込まれたデータの回復が不可能になります。
回復しますか?</translation>
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
