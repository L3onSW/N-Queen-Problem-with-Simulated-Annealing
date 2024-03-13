# N-Queen-Problem-with-Simulated-Annealing
焼きなまし法(シミュレーテッド・アニーリング: Simulated Annealing)を使ってNクイーン問題を解くC言語のプログラムです。
- 置き場に困ったのでGitHubに置きました。
- 世の中に素晴らしい文献が溢れているので、ここではNクイーン問題についての詳細は省略します。

## このプログラムでできること
- 2桁(~99)のNクイーン問題の初期位置から最終的な結果までを綺麗に表示できます。
- コンソールの文字色・背景色を変えられることを利用して、チェス盤を綺麗に表示します。
  - 衝突する位置のクイーンは，赤色背景・黒色文字で表示します。
  - 衝突しない位置のクイーンは，青色背景・黒色文字で表示します。
- E：衝突する位置にいるクイーンの数、t:温度を、チェス盤の上部に表示します。

その他の詳しい中身(変数名など)についてはプログラム中のコメントに書きました。

## 実行方法
以下のようにコンパイルしてから実行します。  
8クイーン問題(N=8)とした場合の実行結果の一例は以下のようになります。 
(実際にはチェス盤に色がつきます。) 
```Console
L3on@MacBook:NQSA$ gcc -Wall -o nqueen_SA nqueen_SA.c -lm
L3on@MacBook:NQSA$ ./nqueen_SA 
6-Queens Problem using Simulated Annealing
E=3, t=30
 6          Q       
 5             Q    
 4                Q 
 3       Q          
 2 Q                
 1    Q             
   1  2  3  4  5  6 
E=1, t=30
 6             Q    
 5          Q       
 4 Q                
 3       Q          
 2                Q 
 1    Q             
   1  2  3  4  5  6 
E=1, t=29
 6    Q             
 5                Q 
 4       Q          
 3 Q                
 2          Q       
 1             Q    
   1  2  3  4  5  6 
E=0, t=28
 6    Q             
 5          Q       
 4                Q 
 3 Q                
 2       Q          
 1             Q    
   1  2  3  4  5  6 
```