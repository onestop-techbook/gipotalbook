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

http://localhost:8080/consoleを開くと、HasuraのConsoleが開きます。開けば成功です。

![Hasura Console](chap-mob-0704/hasuraconsole.png?scale=0.5)


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

次は、模擬データを作ってみます。

上部のリボンから、DATAを開き、Shemaの隣のCreate tableを選択します。

コード化もできるみたいですが、とりあえずはGUI上で作ってみます。

Table Namesがキーになる名称です。

Columnsにいくつかキーを追加していきます。

ID　Integer
User Text
と入れ、Add　Tableをクリックします。これで「箱」ができました。

左のカラムにtable(1) > profileと今作ったテーブルが追加されました。
![テーブルの作成](chap-mob-0704/createtable.png?scale=0.8)

とりあえずは、データを追加してみましょう。

Insert Rowから、IDとNameを入力し、Saveます。型に合わない場合はエラーが返るので修正します。

![データの入力](chap-mob-0704/insertrow.png?scale=0.8)


Browse Rows()から中身の確認が可能です。データ数により後ろの数字は異なります。

![入力データの確認](chap-mob-0704/browserow.png?scale=0.8)

ここまでやったら、次はいよいよQueryを作ってデータの呼び出しを確認します。

GRAPHQLをクリックし、queryを作ります。

```sql
query {
  profile {
    id
    name
  }
}
```

そして、再生ボタンを押すと、queryが走って、保存したデータが呼び出されます。

```sql
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
![GraphQLが動いた](chap-mob-0704/graphql.png?scale=0.8)

めでたしめでたし。Hasuraはすごいですね。

これがソースコード上から呼べると目的の大部分が達せたことになるでしょう。

### Reactから呼び出す
ReactのチュートリアルにGraphQLでの呼び出しについての記載があります。

https://hasura.io/learn/graphql/react/apollo-client/

Set up a GraphQL client with Apolloという章があるので、ここをやってみます。

```sh
$ npm install apollo-boost @apollo/react-hooks graphql
```


でApolloをインストールします。

インストールしている間に、フロントとバックのつなぎ込み職人技が必要なくなりそうです。インフラと設計ができる人が一人いれば、フロントさえいれば完結する未来があるかもしれないねー、といった話が出てきます。きわめて大規模なものはともかく、スタートアップなら全く問題なく動くでしょうし、非常に簡単にできるように思います。

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
読み込みの部分は、profile.jsに外だししておきます。これは、類似の内容を他のページでも利用するのと、Indexが複雑になりすぎるのを避けるためです。

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
 //　成功してデータが帰ってきた時の表示
 return <div>
   {
     data.profile.map(profile => <Profile key={profile.id} profile={profile}/>)
   }
 </div>
}
```

クエリにより、保存したデータを表示することができました。大幅進捗！

## まとめ
恒例のFan Done Learnにも若干かぶりますが、今日のやったことです。

![本日やったこと](chap-mob-0704/done.png?scale=0.8)

