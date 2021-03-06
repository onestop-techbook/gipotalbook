# フロントを作る（モブワーク#7:2020/7/4）

モブワークも7回目を数え、いよいよフロントを作り始めます。
## JS環境を設定する
まずは、JS環境の構築です。いつも通り、Zoomでの画面共有と、VSCodeのLive Shareではじめます。
Shared TerminalでRead/Writeの権限を付けることで、ターミナルも使えるようになりますが、大分バギーなので、結局はターミナルの操作は、なべくらさんだけが行うということにしました。

前回環境をきれいにした状態で、Node.jsを入れました。様々な事情によりnvmでのNode.jsデフォルトバージョンが8になっていましたが、v12.16.3も別途入っているので、新しい方にしましょう。ちなみにこれは理由があって、後ほど原因がわかります。ここでは手動で変更しました。最初からこの状態の方は問題ありません。

```sh
$ nvm use 12.16.3
now using node v12.16.3
$ node -v
v12.16.3
$ pwd
....../tech-book-portal
$ npx create-next-app .
```

Reactだけでなく、Next.jsを使うので `create-next-app` でセットアップをします。

ただ、このとき既存フォルダで作業をしようとして、空のフォルダでないと、コンフリクトする可能性があるというエラーが出ました。

```sh
$ npx create-next-app gipotal
$ mv gipotal/package.json tech-book-portal
$ mv gipotal/public tech-book-portal
$ mv gipotal/pages tech-book-portal
```

そのため既存フォルダから抜け出して、一度新しいフォルダを作り、そこにインストールします。そのうえで `packages.json`と`/pages`と`/public`を対象フォルダ（ここではtech-book-portal）にコピーしました。

```sh
$ cd tech-book-portal
$ npm i
$ npm run dev
```

この `npm i` は `npm install` と同じ効果の短縮コマンドです。`npm run dev` は、`package.json`の`scripts`に書かれている `dev` というコマンドを実行するものです。`yarn` [^yarn]を使う場合は、`npm i` は `yarn` に、`npm run dev` は `yarn dev` に置き換え可能です。

[^yarn]: Facebookが公開したJavaScriptのパッケージマネージャ。npmと互換性がある。

これで、http://localhots:3000にアクセスすると `create-next-app` が作成したデフォルト画面で、動いていることが確認できます。このとき表示されている画面は`/pages/index.js`で、このファイルを書き換えて保存すると、画面にすぐさま反映されます。動作確認のために雑に変更して反映されることを確認します。

初期の`/pages`以下にあれこれ置くのはごちゃっとする問題があります。Next.js 9.1以後は、`/src` というモダンJavaScript界隈で当たり前となっているディレクトリ構造に対応してくれたので、`/src/pages` に移動してしまいましょう。

CTRL+CでNext.jsを止めて作業をします。

```sh
$ mkdir src
$ mv pages src
$ npm run dev
```

表示されることを確認したら、index.jsを適当に整理するとよいでしょう。

![初期のindex.js](chap-mob-0704/indexjs.png?scale=0.5)

この11行目のmainタグが、実際にいま表示している内容なのでここらへんを修正していきますが、そもそも他のあれこれも不要なので、大半をごそっと消してしまいましょう。

```jsx {filename=index.js}
import Head from "next/head";
import styles from "./index.module.css";

export default function Home() {
  return (
    <div className="container">
      <Head>
        <title>Create Next App</title>
        <link rel="icon" href="/favicon.ico" />
      </Head>
      <main>ギポっ</main>
    </div>
  );
}
```

こうなるとCSS定義も消えたので、改めてCSSを当てます。

Reactでよく使われるCSSライブラリはemotion（https://github.com/emotion-js/emotion）などが有名ですが、色々インストールしないといけなくて面倒なので、Next.js標準で使える CSS Modules を使ってみましょう。

まずは `index.module.css` というファイルを作り、ここにCSSを定義します。

```css {filename=index.module.css}
.hello {
  color: red;
}
```

そして、このファイルをJS側で`import`します。また、11行目で本文「ギポっ」にindex.modules.cssで記述しているhelloクラスのCSSが当たり、文字が赤くなります。

```jsx {filename=index.js}
import Head from "next/head";
import styles from "./index.module.css";

export default function Home() {
  return (
    <div className="container">
      <Head>
        <title>Create Next App</title>
        <link rel="icon" href="/favicon.ico" />
      </Head>
      <main className={styles.hello}>ギポっ</main>
    </div>
  );
}
```

ブラウザで確認すると、文字が赤くなったことがわかり、CSSを当てられたということを確認できます。

このとき JSのソースで `styles.hello` は HTML class名として使える文字列なので `<main className={styles.hello}>ギポっ</main>` というコードを記述すると、`index.module.css`で定義されている`hello`クラスのCSSが当たります。

ここで、モジュールとして読み込むところから脱線して、いろいろ話が展開しますが、ここでは省略します。いろいろやり方はあるのですが、シンプルなので、CSS Modulesで行きます。

なお、`.next`の下にいろいろなファイルが勝手に生成されてしまうので、`.gitignore`に`.next`の下を無視するように設定します。

ここでコミットします。なんだかんだで1時間経過してしまいましたが、ここでなべくらさんのマシンにJS環境を作るというところまで完了したことになります。

 * この時点でのGitHubコミットID : 32ed0ed8c5e8cb62cfc39abaef5c773f75c1b3cc [^github200704]

[^github200704]: https://github.com/onestop-techbook/tech-book-portal/tree/32ed0ed8c5e8cb62cfc39abaef5c773f75c1b3cc

## Hasuraを組み込む
目標の二つ目が、「DBを項目少な目でやってみる」です。適当なデータを引っ張ってくるのをテストします。Next.jsにHasuraをどうやって仕込むのかを調べながらやるのです。

Hasura Cloudは結構高額なので、別に立てます。Hasuraはクラウドサービスにも見えますが、Container、その他でも動かせるオープンソースのツールです。なので、まずはHasuraのチュートリアルをやりながら動かしてみます。

そもそもHasura.ioとはなにか、何ができるか？というところから共有が始まります。Hasura.ioは、GraphQLというクエリランゲージでデータベースへの問い合わせができるツールです。

以下の模擬コードでの説明です。一般的なWebAPIでは、それぞれの情報を問い合わせるAPIのURLがあり、そこに問い合わせることでデータを取ってくる事ができます。

```js {caption=RESTをする模擬コード}
// REST URL
const indexUrl = 'https://api.exmaple.com/api/v1/books'
const showUrl = 'https://api.exmaple.com/api/v1/books/12'
const showUrlLight = 'https://api.exmaple.com/api/v1/books/12/light'
const authorUrl = 'https://api.exmaple.com/api/v1/authors/12'

const getBook = (bookId) => {
  const book = getRest(`https://api.exmaple.com/api/v1/books/${bookId}`)
  book.authors = book.authors.map(author => {
    return getRest(`https://api.exmaple.com/api/v1/authors/${author.id}`)
  })
  // 他リソースがあれば、その分だけリクエストを飛ばし続ける
  return book
}
```

ところが、項目が増えたり、関連する要素が増えると、何回もAPIをたたいたり、中間テーブルを作ったりする必要が出てきます。たとえば、Auther自体がネスト構造を持つという特徴を考えます。例えば、Autherの要素として、名前とか、アバターURL、TwitterURLなどが存在します。これを取得するために、RESTでやる場合には、AutherURLのAPIをたたいた後に別のURLをたたいたりと、複数回のアクセスが必要になります。

これに対し、GraphQLであれば、取りたい情報だけをRequestとして指定できるので、シンプルにデータを取得することができます。

```js
const graphqlUrl = 'https://api.example.com/api/graphql'
const query = gql`
query getBooks {
  title
  price
  author {
    name
    avatarUrl
    twitterUrl
  }
}

graphqlClient(graphqlUrl, query)

// データ例
[
  {
    title: 'ワンストップバックアップ',
    price: 100,
    author: {
      name: '親方Project',
      avatorUrl: 'https://...',
      twitterUrl: 'https://...'
    }
  },
  ....
]
```

このように、リクエスト一発で必要な情報を取って来ることができます。

既存のREST APIがあるのであれば、通常は Apollo server [^server]を使って、REST API とフロントエンド（ウェブとは限りません。iOSやAndroidもあります）を仲介する BFF（Backends For Frontends）を建てて GraphQL を実現するのが定番ですが、今回は完全新規なため、せっかくなので Hasura を使います。

[^server]: https://www.apollographql.com/docs/apollo-server/

Hasura は、PostgreSQLと密結合して、DBをいい感じにダイレクトに GraphQL とつなぎ込んでくれるフレームワークです。

### Hasura.ioを動かしてみる

Hasuraを使うには、有料のHasuraのサーバーを使ってもよいし、Dockerを使って手元でPostgreSQLと一緒に立ち上げることもできます。今回は、一旦手元で動かしてみることにします。

Quick start with dockerというチュートリアルがあるので、これを使います。

https://hasura.io/docs/1.0/graphql/manual/getting-started/docker-simple.html

作業フォルダのルートで動かして問題なさそうなので、ここで動かします。

```sh
$ wget https://raw.githubusercontent.com/hasura/graphql-engine/stable/install-manifests/
docker-compose/docker-compose.yaml
# または
$ curl https://raw.githubusercontent.com/hasura/graphql-engine/stable/install-manifests/
docker-compose/docker-compose.yaml -o docker-compose.yml

$ docker-compose up -d
# デーモンとして起動

$ docker ps
# 起動していることを確認。
```

http://localhost:8080/consoleを開くと、HasuraのConsoleが開きます。開けば成功です。

![Hasura Console](chap-mob-0704/hasuraconsole.png?scale=0.5)


#### [column] Windows 10でDockerで立てたはずのConsoleが開けない
Windows10（Home Edition）でHasura.ioのDocker composeをやってみたところ、動いているはずなのにConsoleにアクセスできないという現象が発生しました。

Windows Firewallを切ってみましたがダメ。結果としては、Virtual Machineのポート変換ができていないということがわかりました。

参考ページ：https://teratail.com/questions/151484

VirtualBoxを使っていたので、VirtualBoxマネージャの設定→ネットワーク→高度→ポートフォワーディングでルールを追加します。

![VMネットワーク設定](chap-mob-0704/VMportsetting1.png?scale=0.8)

![ポートフォワーディングの追加](chap-mob-0704/VMportsetting2.png?scale=0.8)

親OSのポート←VMのポート←Dockerのポートのような構造になっているようで、ポートフォワーディングの設定が必要なようです。

つまり、Dockerで別のツールを使うたびに、必要に応じて新しいポートの設定を追加しなきゃいけないようですね。

#### [/column]

#### [column] いまのWindows10ならWSL2が楽かも

いまのWindows10ではWSL2でDockerがめちゃくちゃ楽に使えるので、旧来のVirtualBoxやDockerを削除して、WSL2上でdockerを動かす方が楽かもしれません。

#### [/column]

次は、模擬データを作ってみます。

上部のリボン(メニューバー)から、DATAを開き、Shemaの隣のCreate tableを選択します。

コード化もできるみたいですが、とりあえずはGUI上で作ってみます。

Table Namesがキーになる名称です。

Columnsにいくつかキーを追加していきます。

ID　Integer
User Text
と入れ、Add　Tableをクリックします。これで「箱」ができました。

左のカラムにtable(1) > profileと今作ったテーブルが追加されました。
![テーブルの作成](chap-mob-0704/createtable.png?scale=0.8)

とりあえずは、データを追加してみましょう。

Insert Rowから、IDとNameを入力し、Saveします。型に合わない場合はエラーが返るので修正します。

![データの入力](chap-mob-0704/insertrow.png?scale=0.8)


Browse Rows()から中身の確認が可能です。データ数により後ろの数字は異なります。

![入力データの確認](chap-mob-0704/browserow.png?scale=0.8)

ここまでやったら、次はいよいよQueryを作ってデータの呼び出しを確認します。

GRAPHIQLをクリックし、queryを作ります。

```gql
query {
  profile {
    id
    name
  }
}
```

そして、再生ボタンを押すと、queryが走って、保存したデータが呼び出されます。

```gql
{
  "data": {
    "profile": [
      {
        "ID": 1,
        "Name": "なべくら"
      },
      {
        "ID": 2,
        "Name": "KANE"
      },
      {
        "ID": 3,
        "Name": "おやかた"
      }
    ]
  }
}
```
![GraphiQLが動いた](chap-mob-0704/graphql.png?scale=0.8)

めでたしめでたし。Hasuraはすごいですね。

これがソースコード上から呼べると目的の大部分が達せたことになるでしょう。

### Reactから呼び出す
ReactのチュートリアルにGraphQLでの呼び出しについての記載があります。

https://hasura.io/learn/graphql/react/apollo-client/

ReactでGraphQLを使うには、Apolloというライブラリを使います。チュートリアルにSet up a GraphQL client with Apolloという章があるので、ここをやってみます。

```sh
$ npm install apollo-boost @apollo/react-hooks graphql
```

でApolloをインストールします。

インストールしている間に、「フロントとバックのつなぎ込み職人技が必要なくなりそう。」「インフラと設計ができる人が一人いれば、フロントさえいれば完結する未来があるかもしれないねー。」といった話が出てきます。きわめて大規模なものはともかく、スタートアップなら全く問題なく動くでしょうし、非常に簡単にできるように思います。

src/pages/index.js に以下のコードを追加します。

```js
 import ApolloClient from 'apollo-client';
 import { InMemoryCache } from 'apollo-cache-inmemory';
 import { HttpLink } from 'apollo-link-http';
 import { ApolloProvider } from '@apollo/react-hooks';

 const createApolloClient = () => {
 return new ApolloClient({
   link: new HttpLink({
     uri: 'http://localhost:8080/v1/graphql',
   }),
   cache: new InMemoryCache(),
 });
};
```
読み込みの部分は、profile.jsに外だししておきます。これは、類似の内容を他のページでも利用するのと、index.jsが複雑になりすぎるのを避けるためです。

```js
import gql from 'graphql-tag';
import {useQuery} from '@apollo/react-hooks';

// クエリ
const OUR_FIRST_QUERY = gql`
 query {
   profile {
     id
     name
   }
 }
`;

const Profiles = () => {
 const { loading, error, data } = useQuery(OUR_FIRST_QUERY)
 // ローディング中の表示
 if (loading) return <p>loading</p>
 // エラー時の表示
 if (error) return <p>{error.toString()}</p>
 //　成功してデータが返って時の表示
 return <div>
   {
     data.profile.map(profile => <Profile key={profile.id} profile={profile}/>)
   }
 </div>
}
```

クエリにより、保存したデータを表示することができました。大幅進捗！

 * この時点でのGitHubコミットID : c365515edccb0c370d0ec6b1dc417d8a3b9f4759 [^github200704-2]

[^github200704-2]: https://github.com/onestop-techbook/tech-book-portal/tree/c365515edccb0c370d0ec6b1dc417d8a3b9f4759



## まとめ
恒例のFun Done Learnにも若干かぶりますが、今日のやったことです。

Hasuraしゅごい、の一言に尽きますね。モブでやることで、参加者全員がまんべんなく理解しつつ進められます。データベースからデータを引っ張ってきて、画面に表示するところまでで来たのが本日のモブワークでした。あとは本当に実装していくだけなのでは？

7回のモブワークを経てここまできました。

![本日やったこと](chap-mob-0704/done.png?scale=0.8)

![7月4日のFun Done Learn](chap-mob-0704/0704fundonelearn.png?scale=0.8)
