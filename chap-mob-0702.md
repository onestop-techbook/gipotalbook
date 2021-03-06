# データの要件を考えた（モブワーク#6:2020/7/2）

本日のモブワークは、データ構造の相談です。本やサークルにどういうデータを持つのか、その相談です。設計の肝の一つかもしれません。

## どうやって作る？

トップダウンで作る、ボトムアップで作る、という話が出ますが、そもそもトップダウンって何？トップダウンで作るって何？という話でひとしきり盛り上がります。

一旦、ボトムアップを要素・要件から構築していく手順とし、トップダウンを想定する画面から主キーを抽出していく形と定義します。

この間、この相談と、自社ではこう、MS-Accessでやってるよ、全部をAccessで完結させるのが社内あるある、などの話が出ました。情シス的には、雑なメモを持ってこられて、これを実装してくれと言われることもあるなど、本筋からずれる話がいっぱい出てきます。

今回の実装はトップダウンで行うことにしました。直感的ですし、抜け漏れがあると後からやるのは手戻りが怖いので、画面を想定しながら、主になるキーを抽出していく形としました。そして、一回やってみて、最悪手戻りがあってもそれを許容するということで合意して進めることにしました。

## 同人誌の要素を抽出する
まずは、関連しそうなキーワードをみんなで書き出します。上位概念としてはサークルに関する情報、本に関する情報などいくつかの親になるキーがあり、より詳細な情報としてそこに含まれるであろうキーが存在します。たとえば、本に関して言えば、書名、サークル名、ページ数、発行日、判型などが該当します。ただし現時点では親子関係についてはあまり深く考えず、関連するワードをみんなでぺたぺた貼っていきます。

![Step1:キーの書き出し](chap-mob-0702/key1.png?scale=0.8)

書き出してみると。約70個のワードが書き出されました。思っていたよりたくさんあります。

次に、上位概念となるワードをいくつか選び、それぞれのワードをそこに所属させる形で分類します。

一旦、サークル、本、イベントが上位であろうということになり、分類作業を進めます。

![Step2:キーの分類](chap-mob-0702/key2.png?scale=0.8)

サークルに関するタイルは13個、本に関するタイルは40個、その他数個ずつになります。やはり本の情報が多いですね。

上位概念は、サークル、本、イベント、が順当に選ばれます。

次に進む前に、オブジェクトとして足りているかを確認します。たくさん出ているところはよいとして、少ないところに足りていない概念がないかを確認します。

#### [column] おやかたAI
ここで、10年後にこの付箋を見たらDBのプログラミングをやってくれるAIができたりしないかな、という話から、過去のツイートから目次を生成してくれるAIができないかな、という話に脱線します。目次を作ってくれて、執筆のペースをコントロールしてくれる（お尻を叩いてくれる）AIです。目次作ってくれるAIはいいですね。

会話の中から、ネタを見つけていくのは普段、私、おやかたがやっていることです。

妖怪本書いてけTwitterBot:FF外から失礼します。目次作りました。本書いてみませんか？

なかなか圧が強いですね…ヘタするとウイルス認定されかねないｗ

#### [/column]

## リレーションを考える
ジャンルとカテゴリというキーワードのタイルが浮いていたことから、タグやジャンル、リレーションについての話に移動します。

本にタグ付けし、関連するタグ（上位概念、下位概念、近接するタグ）が当該タグのページに表示されます。本を探しているひとが、自分に関連するキーワードを考え、探す必要があります。知らない概念については、関連ワードを見つけ出すのも大変です。おすすめタグとして書いてあれば簡単にチェックできます。

たとえば、フロントエンドというタグについて、TS（TypeScript）、React、Vueなどが関連するタグとして挙げることを想定しています。親子関係だけではなく、対立するワードも含まれて欲しいところです。たとえば、青銅器時代が鉄器時代になった歴史を例にとれば、青銅器時代というタグのページがあったとして、おすすめタグとして、石器時代、鉄器時代、という、ある種対立するワードが含まれるというイメージです。青銅器時代のタグのページの中に、石器時代、鉄器時代というワードを含めることで、次の/前の時代について調べることもできます。フレームワークのマイグレーションを考えるなどに役に立つと考えています。タグのクリックだけで上下左右に移動できることを期待しています。

したがって、タグどうしの関係をなんらかの形で保持する必要が生まれます。ただし、現時点でその関係をどう持つ、あるいは実装するのか、ということは考えません。

この関係性について、実装をどうするかとは別として、その概念について、いくつかの図を作りながらの概念共有がなされます。

関連という形を表すにもいくつかの方法があることは明らかです。それを図的に表したものがいくつかできます。キーワードの関係をベクトル図的に表すもの、上位概念を親として並列的・階層的に並べる場合、そしてマトリクス的に関係の重みを設定する場合などが考えられます。実装についてのアイディアも出つつ、わいわい話を進めます。

![リレーションの表し方のアイディア](chap-mob-0702/relation.png?scale=1.0)


データベースとして持つのか、というのはあくまで実装の話ですから、こういう課題があるという点を共通認識として有しておけばOKです。タグの関係を矢印としてリレーションを持たせるとか、親子関係を主体としたリレーションを持たせる、あるいはマトリクスとして重みをもたせる、といったアイディアが出て、一旦打ち切りました。

ここで付記しておくべきは、「タグのテーブルは運営で管理する」でしょう。自由入力とすると粒度もばらつき、細分化しすぎてしまいます。また名寄せ、表記ゆれなどの問題も生じますし、先述のリレーションが組み合わせ爆発してしまいます。どのような実装を取るにしても、少なくともタグ数の2乗に近い組み合わせが生じてしまうのです。

なお、タグのテーブルを運営で管理するということについては、コミケのジャンルの扱いに近いという指摘もうなづくところがあったのでここに共有しておきます。確かにコミケのジャンルは準備会（運営）が定めており、参加日程や配置を定める重要な指標です。サークル数などの流行り廃りにより適宜見直し、ジャンル再編が行われるものの、その権限は全面的に準備会にあり、ユーザー(サークル参加者)には権限がないとともに、ジャンルを違えるのは書類不備にもなりえるものです。

タグのテーブルは公開しない、あるいは粒度をどうするか、といった話しは運用の話なので、これも補足で書いておきます。せっかく議論した内容なので、残しておきます。良い議論ができていますね。

## テーブルごとに見ていく
ユーザー情報（サークル主の情報）、サークル、本などの上位概念といえるキーワードが整理されたところで、それぞれのテーブルを詳細にみていきます。

ここでの議論のポイントは、サークルというユーザーが存在するのか、ユーザーにサークルが紐づくのか、です。基本的には両者は同一のものであるかと思われるかもしれませんが、複数のサークルに参加している、寄稿しているといった事象が生じます。その時、書籍情報や参加イベントといったコアな情報の編集権限をどう扱うか、といったところに議論が生まれます。

特に、今回の運営メンバーは、サークルを二つ持っている、あるいはいろいろなサークルに寄稿しているなどがざらにあります。親方Projectもたくさんのサークル主から寄稿してもらっています。なお、逆にサークル主じゃない人もいますよ。

![サークルとユーザーの関係](chap-mob-0702/circle.png?scale=0.5)


そこで、サークルの扱いを決める必要があります。「サークル」というユーザーを作るという方法が一つ考えられます。しかし、複数人のメンバーがいる場合は、IDやパスワードの共有が必要になり、セキュリティ上のリスクが生じます。そこで、基本方針としては、ユーザーにサークルを紐づける方向で考える形となりました。メリットとしては、ユーザーID等の共有が不要であること、編集権限を任意につけることができるなどが挙げられます。新規のメンバーが入っても登録が簡単ですし、サークルページの編集権限をつけたり外したりも可能です。

ユーザーとサークル回りの情報を整理したところで、そろそろ時間です。必要そうな情報はだいたい洗い出せたと思います。

![サークルとユーザーに関する情報のテーブル](chap-mob-0702/usertable.png?scale=1.0)

## モブワーク#6のふりかえり
今回は、データベース設計の基本ということで、要素の抽出と整理をしました。今回のようなブレスト的に要素抽出して整理していくワークにおいては、MURALの有用性が最大限発揮されたといえるでしょう。

![#6のFun Done LearnとNext work](chap-mob-0702/0702fundonelearn.png?scale=0.9)

データベース設計の入り口ともいうべき、キーの抽出やリレーションの設計が一応できました。

みんながMURALに慣れてきて、カレンダー機能やここ注目機能、さらにはエリア固定といったいろいろな機能の使いこなしが少しずつできてきました。ワンストップMURALの気運？

次のワークは、フロント環境の設定と、少な目な項目でDB構築のプロトタイプをやってみよう、ということになりました。素晴らしい進捗です。
