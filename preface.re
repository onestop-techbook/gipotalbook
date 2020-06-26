= はじめに

== この本の目的

この本を手にとっていただき、ありがとうございます。

先ごろ始まった「技術同人誌ポータル　ギポタル」の開発過程を追う本です。この本の目的は次に示すようにいくつかありますが、いずれも開発者一同及びこの本を手に取っていただいた皆様に役に立つことを信じています。

 1. ギポタルを紹介、周知するため

このサービスの開発過程をご紹介することで、よりたくさんの人にぎぽたるを知って、使って欲しいと思っています。サークル主にとっては、サークルのポータルとして。またここを経由してご自分の本の売り上げがUPすることを期待しています。

読者の皆様にとっては、新しい本との出会いの場所として、あるいはお気に入りサークルを見つけるために。

この本で、コンセプトなどを知って、使って、口コミで広がれば最高です。また、機能追加などの要望についてはぜひお寄せください。みんなで作り上げるサイトです。

 2. 開発のふりかえりとして

今回のサービスは、「あったらいいな」から始まっています。類似のサービスにあること、既存サービスでは足りないこと、そもそも何のためのサービスかといったコンセプトを決めるところからメンバー一同で作り上げてきました。

私たちがどういうことを考えて開発を進めてきたか、そこで得たものはなんだったのか、使った技術やツール、技術について整理することで、開発そのもののふりかえりと、類似の開発をされる方への参考になれば幸いです。

バックグラウンドもスキルもレベルも異なる人が任意で集まったチームでの開発です。実際のチームビルディングの参考になると幸いです。

 3. Webサービス開発の一実例として

Webサービスを開発するためには、さまざまな技術スタックが必要です。一人で全てできるわけではない場合もあるでしょう。一連の流れとして全てを記録することで、それぞれのステップ、コアテクノロジーに関するインデックスとなるように留意しています。ある領域の人にとっては当たり前の内容も多いでしょう。ですが専門外の人にとってはまったく分からない内容も含まれます。

もちろんその技術やステップに必要なこと全てを解説することはできませんが、開発のひとつのルートとして、マイルストーンをできるだけ配置するようにしています。これが最善であるという保証はありません。またそれぞれの技術についても複数の参考書、書籍、さまざまな媒体で参考事例や解説があるような広大かつ深淵なものですから、この本に取り上げることができた内容なそのごく一部にすぎません。ですがそれぞれのステップにおいて、必要な本当の最低限を知ることで、それをベンチマークとしてさらにスマートに開発を進めることができる場面があるのではないかと考えます。

　

さて、本稿執筆者の皆様、そしてギポタル開発チームの皆様、本当にありがとうございます。思いつきから始まったこのプロジェクトですが、サービスのリリースと本の発行を経て、一区切りとなります。当然一人では単なる夢物語に過ぎませんでしたが、全員が参加することで形になりました。

最後に、この本ができあがるがるころには、サービスがリリースされているはずですが、まだまだあったらいいなの機能や働きがあるでしょう。登録サークル数が増え、訪れる人が増えることでさらに進化していきます。サークル登録する、口コミで広げる、実際に使う、いずれもとてもありがたいContributeです。要望を出す、開発チームに参加する、ぜひお願いします。




//flushright{

2020年5月

編集長　親方＠親方Project　拝

//}


== 免責事項

 * 本書の内容は、情報提供のみを目的としております。著者一同、正確性には留意しておりますが、正確性を保証するものではありません。この本の記載内容に基づく一切の結果について、著者、編集者とも一切の責任を負いません。
 * 会社名、商品名については、一般に各社の登録商標です。TM表記等については記載しておりません。また、特定の会社、製品、案件について、不当に貶める意図はありません。
 * 本書の一部あるいは全部について、無断での複写・複製はお断りします。