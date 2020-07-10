# フロントを作る(モブ#7 7/2)

モブワークも7回目を数え、いよいよフロントを作り始めます。

## JS環境を設定する
まずは、JS環境の構築です。いつも通り、Zoomでの画面共有と、VSCodeのLive Shareではじめます。
Shared TerminalでRead/Writeの権限を付けることで、ターミナルも使えるようになります。

前回環境をきれいにした状態で、node.jsを入れました。デフォルトがバージョン8になっていたので、これを新しいのにします。v12.16.3も別途入っているので、新しい方にしましょう。

```sh
$ nvm use 12.16.3
now using node v12.16.3
$ node -v
v12.16.3
$ npx create-next-app --template with typescript .
```
Reactだけでなく、Next.jsも使うので、入れる方針で行きます。

空のフォルダでないと、コンフリクトする可能性があるというエラーが出ます。ですから、一つ上の階層に上がり、新しいフォルダを作り、そこにインストールします。そのうえで、package.json /pages /publicを対象フォルダ(ここでは/tech-book-portal)にコピーします。

```sh
$ nvm install
$ npm rum dev
```

これで、localhots:3000にアクセスすると、動いていることが確認できます。

/pages/index.jsが表示されているファイルですから、適当に変更して反映されることを確認します。

ルートはごちゃごちゃしがちなので、/src/pages/に移動しちゃいましょう。Ctrl+Cで一度止めて、npm run devで再起動してやると、反映されます。

表示されることを確認したら、index.jsを適当に整理するとよいでしょう。

Mainの中が現在表示しているものなので、Main の中を書き換えれば任意に書き換えできます。

StyleやFooterの中身も不要なので消してしまいます。Styleの直書きなどの話も出てくるのですが、一旦わきにおいておきます。Styleを全消しすると、Styleが当たらなくなるので、中央寄せから、左上に寄ることになりますので、一旦全消しして確認します。

cssを外出しするには、index.cssを作り、これをインポートします。

```js
import styles from './index.module.css'

    <main className={styles.hello}>
    ギポっ
    </main>
```

index.module.css

```css　
.hello {
  color: red;
}
```

これで、文字が赤くなるはずです。普通のcssはjsで読めないのですが、module.cssと書いてモジュールとしてインポートすることができます。

ここで、モジュールとして読み込むところから脱線して、いろいろ話が展開しますが、ここでは省略します。いろいろやり方はあるのですが、シンプルなので、css.moduleで行きます。

なお、.nextの下にいろいろできてしまうので、.gitignoreに.nextの下を無視するように設定します。

ここでコミットします。なんだかんだで1時間経過してしまいましたが、ここで鍋倉さんのマシンにJS環境を作るというところまで完了したことになります。

## Hasuraを組み込む
目標の二つ目が、DB項目少な目でやってみることにします。適当なデータを引っ張ってくるのをテストします。Next.jsにHasuraをどうやって仕込むのかを調べながらやるのです。

Hasura Clowdは結構高額なので、別に立てます。Hasuraha、クラウドサービスにも見えますが、Container、その他で動かせるオープンソースのツールです。なので、まずはHasuraのチュートリアルをやりながら動かしてみます。

そもそもHasuraとはなにか、Hasura.ioは何ができるか？というところから共有が始まります。これは、GraphQLというクエリランゲージでデータベースへの問い合わせができるツールです。

以下の模擬コードでの説明です。一般的なWebAPIでは、それぞれの情報を問い合わせるAPIのURLがあり、そこに問い合わせることでデータをとってこれます。

ところが、項目が増えたり、関連する要素が増えると、何回もAPIをたたいたり、中間テーブルを作ったりする必要が出てきます。たとえば、Auther自体がネスト構造を持つという特徴を考えます。例えば、Autherの要素として、名前とか、アバターURL、TwitterURLなどが存在します。これを取得するために、RESTでやる場合には、AutherURLをたたいた後に別のURLをたたいたり、複数回のアクセスが必要になります。

```
// REST URL
const indexUrl = 'https://api.exmaple.com/api/v1/books'
const showUrl = 'https://api.exmaple.com/api/v1/books/12'
const showUrlLight = 'https://api.exmaple.com/api/v1/books/12/light'
const authorUrl = 'https://api.exmaple.com/api/v1/authors/12'
const graphqlUrl = 'https://api.example.com/api/graphql'
// Request
// {
//   books {
//     title
//     price
//   }
// }
// {
//   book {
//     title
//     price
//     description
//     author {
//       name
//       avatarUrl
//       twitterUrl
//     }
//   }
// }
// Result
const result = [{
  title: 'ワンストップバックアップ',
  price: 100,
  author: {
    name: '親方Project',
    avatorUrl: 'https://...',
    twitterUrl: 'https://...'
  }
}, {
  title: 'ワンストップバックアップ',
  price: 100,
},
]
```

これに対し、GraphQLであれば、取りたい情報だけをRequestとして指定できるので、シンプルにデータを取得することができます。

通常はAppoloというバックエンドとフロントエンドの中間を媒介するBFFを立てて、GraphQLを作ります、これに対してのHasuraの利点は、直接DBにアクセスすることができ、GraphQLとDBをいい感じにつなぎこんでくれるシステムであるところです。項目の増減もRequestの項目をいじるだけでよくなります。

ある意味では、データベース構造に寄らないデータの扱いが可能になるともいえるかもしれません。

### Hasura.ioを動かしてみる

有料のHasuraのサーバーを使ってもよいし、Dockerを使って手元でPostgreSQLと一緒に立ち上げることもできます。ですから、一旦手元で動かしてみることにします。

Quick start with dockerというチュートリアルがあるので、これを使います。

https://hasura.io/docs/1.0/graphql/manual/getting-started/docker-simple.html

Tech-book-portalのルートで問題なさそうなので、ここで動かします。

```
wget https://raw.githubusercontent.com/hasura/graphql-engine/stable/install-manifests/
docker-compose/docker-compose.yaml

または

curl https://raw.githubusercontent.com/hasura/graphql-engine/stable/install-manifests/
docker-compose/docker-compose.yaml -o docker-compose.yml

$ docker-compose up -d
デーモンとして起動

$ docker ps
起動していることを確認。

```

http://localhost:8080/consoleを開くと、HasuraのConsoleが開きます。

#### [column] Window10でDockerでたてたはずのConsoleが開けない
Windows10(Home)でHasura.ioのDocker composeをやってみたところ、動いているはずなのにConsoleにアクセスできないという現象が発生しました。

Windows Firewallを切ってみましたがダメ。結果としては、Virtual Machineのポート変換ができていないということがわかりました。

参考ページ：https://teratail.com/questions/151484

VirtualBoxを使っていたので、VirtualBoxマネージャの設定>ネットワーク→高度→ポートフォワーディングでルールを追加します。


![VMネットワーク設定](chap-mob-0704/VMportsetting1.png?scale=0.8)

![ポートフォワーディングの追加](chap-mob-0704/VMportsetting2.png?scale=0.8)

親OSのポート←VMのポート←Dockerのポートのような構造になっているようで、ポートフォワーディングの設定が必要なようです。

ということは、Dockerで別のツールを使うたびに、必要に応じて新しいポートの設定を追加しなきゃいけないようですね。

#### [/column]

