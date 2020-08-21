# サーバーにデプロイ　モブ#13　8/13
いよいよ今日は本番サーバーにデプロイします。

HerokuにTeamを作り、Hasuraを組み込みます。また、Vercelを使って自動デプロイするところが今日の目標です。

## HerokuにTeamを作りデプロイする
Hasura/Graphqlをデプロイします。これ自体は前回いちどやってますが、個人IDでの実施でしたから、まずはHerokuにTeamを作ります。

Teamにすることで、複数の人手アクセスできます。5人までは無料ですので、ユーザーを招待します。

再度、Hasuraからワンクリックでのデプロイができますので、今回もこれを使います。

名前をGipotalとして、Hasuraをデプロイします。

![herokuへデプロイ](chap-mob-0813/heroku1.png?scale=0.8)

デプロイにはしばらく時間がかかりますので、しばし待ちます。

その間に、Vercelを準備します。これは、連携したGitHubリポジトリを自動でビルドしてくれたり、デプロイしてくれたりする素敵なツールです。

大変便利なのですが、FreeプランではOrganizationを連携させることはできないので、有料プランとしました。月20ドルというのが少し痛いところではありますが、むしろ開発とマネタイズへのモチベーションとなりますので、さくっと払ってしまいます。

GitHubの連携設定をはじめます。


![Vercelの設定](chap-mob-0813/vercelSetting1.png?scale=0.8)

GitHubのリポジトリを選択し、Deployします。

```js package.json 
    # Scripts内
    "prebuild": "npm run codegen",
```

また、codegen.yml内のSchemaURLをHerokuのHasuraのGraphqlのURLに書き換えます。

これで再度走らせると、インストールが進みますが、Resource not foundが返ってきます。DB本体が存在しないためのエラーですから、再度手動ポチポチでデータを入れましょう。

![Vercelの設定完了](chap-mob-0813/verceldone.png?scale=0.8)

入力されたデータが表示されたので、一旦これでOKですね。

