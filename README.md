# 🌌 Aliens: Resistance War

Thử nghiệm game: https://drive.google.com/file/d/1z4jYRFalWQ0yAWDxGbnwA_87EKYjz_A_/view?usp=sharing

# Giới thiệu game
Aliens: Resistance War là một tựa game bắn tàu vũ trụ không gian 2D với nhịp độ nhanh và dồn dập, được phát triển hoàn toàn bằng thư viện SDL2 — một thư viện lập trình đa nền tảng phổ biến. Trong trò chơi này, bạn sẽ hóa thân thành một phi công đơn độc đang điều khiển một con tàu vũ trụ chiến đấu với tất cả những gì còn lại để sinh tồn. Kẻ thù của bạn là những thế lực ngoài hành tinh hung hãn, thiên thạch không gian chết chóc, những con boss khổng lồ sở hữu sức mạnh hủy diệt, và các hiện tượng vũ trụ kỳ bí chưa từng được biết đến.

![Gameplay Preview](image/ARWBackground.png)

# 1. Gameplay:
- Bạn sẽ vào vai người sống sót cuối cùng trong một lực lượng kháng chiến, điều khiển một chiếc tàu không gian tối tân giữa lòng vũ trụ bao la, nơi hiểm họa rình rập từ mọi phía.
- Nhiệm vụ của bạn là bắn hạ tất cả kẻ địch, từ những con tàu nhỏ cho đến các boss khổng lồ, nhằm ghi điểm và nhận được các vật phẩm hỗ trợ quý giá giúp bạn sống sót lâu hơn.
- Trong quá trình chơi, bạn sẽ liên tục phải đối mặt với làn đạn dày đặc từ kẻ thù, các thiên thạch rơi không ngừng, và những kỹ năng bất ngờ từ boss – tất cả đều đòi hỏi bạn phải có phản xạ nhanh nhạy và khả năng di chuyển linh hoạt.
- Trò chơi không có kết thúc cố định; bạn sẽ chiến đấu đến khi không còn sức lực, với mục tiêu cuối cùng là đạt được số điểm cao nhất có thể.

# 2. Các cơ chế chính:
- Khi bắt đầu game: Có màn hình menu với nút start và background lấy bối cảnh ngoài không gian. Ấn vào nút Play để bắt đầu trò chơi.
  ![Gameplay Preview](image/ARWPlayButton.png)
- Di chuyển & Bắn: Người chơi điều khiển phi thuyền bằng các phím (A, S, D, W) hoặc các phím mũi tên và có thể bắn bằng phím Space với tốc độ di chuyển, tốc độ và sát thương đạn tùy theo buff đang hoạt động.
  ![Gameplay Preview](image/mechanism.png)
- Kẻ thù: Các loại enemy di chuyển theo các quỹ đạo khác nhau và tấn công bằng đạn. Càng về sau, game sẽ có boss xuất hiện với kỹ năng và cách tấn công đặc biệt.
  ![Gameplay Preview](image/enemy1.png)
  ![Gameplay Preview](image/enemy2.png)
  ![Gameplay Preview](image/enemy3.png)
  ![Gameplay Preview](image/enemy4.png)
  ![Gameplay Preview](image/enemy5.png)
- Boss Fight: Boss xuất hiện định kỳ sau mỗi 30–60 giây và có thanh máu riêng trên màn hình. Có bốn loại boss với các phương thức tấn công khác nhau:
  + ![Gameplay Preview](image/boss1.png) Boss 1: bắn loạt đạn tỏa ra các hướng với giới hạn góc 180 độ về phía dưới.
  + ![Gameplay Preview](image/boss2.png) Boss 2: bắn đạn lớn về phía player, gây sát thương cao.
  + ![Gameplay Preview](image/boss3.png) Boss 3: bắn 3 viên đạn đuổi player trong khoảng thời gian ngắn.
  + ![Gameplay Preview](image/boss4.png) Boss 4: bắn đạn nảy bốn lần rồi mới ra khỏi màn hình.
- Meteor: Thiên thạch rơi ngẫu nhiên gây sát thương, gây khó khăn trong di chuyển.
  ![Gameplay Preview](image/meteor.png)
- Buff hệ thống: Tiêu diệt kẻ thù có thể rơi ra buff hỗ trợ với các hiệu ứng như:
  ![Gameplay Preview](image/buffAppear.png)
  + HEAL: Hồi máu
  + SHIELD: Khiên bảo vệ
  + DAMAGE BOOST: Tăng sát thương
  + ATTACK SPEED: Tăng tốc độ bắn
  + UP MAX HP: Tăng máu tối đa
  + REFLECT: Phản lại đạn địch
  + FREEZE: Đóng băng enemy
  + SPEED BOOST: Tăng tốc di chuyển
- Khi player bị tiêu diệt, người chơi có thể chọn R để tiếp tục hoặc Q để thoát.
  ![Gameplay Preview](image/endgame.png)

# 3.  Tính năng khác
- Thanh máu động cho cả Player và Boss.
  ![Gameplay Preview](image/hp.png)
- Hiệu ứng lửa động phía dưới phi thuyền.
- Hiển thị điểm số và buff đang kích hoạt (+5 điểm khi tiêu diệt enemy, +20 điểm khi tiêu diệt boss).
  ![Gameplay Preview](image/point.png.png)
  ![Gameplay Preview](image/buffDisplay.png)
- Xử lý va chạm thông minh cho đạn, địch, buff, thiên thạch,...
- Hiệu ứng kết thúc Game Over với điểm số và hướng dẫn thao tác tiếp theo.

# 4. Về Source code:
- 📂 draw:
  + Khởi tạo cửa sổ và renderer
  + Sử dụng SDL_CreateWindow và SDL_CreateRenderer để tạo cửa sổ và renderer.
  + Sử dụng các thư viện của SDL2
  + Bao gồm: SDL2, SDL2_image, SDL2_ttf, SDL2_mixer.
  + Dùng để: load ảnh (IMG_Load, SDL_CreateTextureFromSurface), font chữ (TTF_OpenFont, TTF_RenderText_Solid), âm thanh (Mix_LoadWAV, Mix_PlayChannel).
  + Vẽ lên màn hình
  + Gồm các hàm như renderScore, renderHealthBar, renderGameOver, renderBuffText để vẽ các đối tượng UI và trạng thái game.
    
- 📂 elements:
  + `Player: điều khiển tàu vũ trụ người chơi, xử lý buff, máu, va chạm.
  + `Bullet` / `EnemyBullet` / `Type0~3Bullet`: quản lý đạn của player, boss, enemy.
  + `Enemy`: kẻ địch thông thường, có di chuyển và tấn công.
  + `Boss`: boss đặc biệt, nhiều loại với kiểu tấn công riêng.
  + `Meteor`: thiên thạch rơi từ bốn phía.
  + `Fire`: hiệu ứng lửa phía sau tàu.
  + `Buff`: hiệu ứng hỗ trợ (tăng tốc, hồi máu, phản đạn...).
  + Tất cả các thành phần trên được quản lý bằng các vector và được update/render mỗi khung hình.

- 📂 elements_actions:
  Xử lý tương tác giữa các thành phần
  + Va chạm giữa đạn và enemy, boss.
  + Player nhặt buff, chịu sát thương từ bullet, meteor, black hole.
  + Hàm `spawn` tạo enemy mới khi số lượng < 5.
  + `bossSpawning` sinh boss theo thời gian ngẫu nhiên.
  + `spawnMeteor`, `spawnBlackHole` tạo thiên thạch và hố đen sau thời gian định sẵn.
  Quản lý buff
  + `applyBuffToPlayer` dùng để kích hoạt hiệu ứng buff lên player.
  + Buff có thời gian hiệu lực, hiển thị chữ thông báo ngắn.

- 📂 events:
  Xử lý sự kiện bàn phím và game loop chính
  + Nhấn phím để di chuyển tàu (`handleEvent` của `Player`).
  + Bắn đạn bằng phím `SPACE`.
  + Menu chọn bắt đầu chơi (`Menu`), hoặc trở lại khi thua (`R` để chơi lại, `Q` để thoát).
  
  Quản lý vòng lặp game
  + Vòng lặp chính chạy `while (!quit)` để xử lý mọi frame.
  + Phân nhánh giữa hiển thị menu (`menu.menuOrGame()`) và gameplay.

- 📂 music:
  Xử lý âm thanh
  + `gSound`: âm thanh bắn trúng.
  + `endGame`: âm thanh thua cuộc.
  + Dùng `Mix_LoadWAV`, `Mix_PlayChannel`, `Mix_VolumeChunk`.

- 📂 player:
  Xử lý dữ liệu người chơi
  + Biến `playerPoint`: điểm số hiện tại.
  + `Player`: lớp xử lý buff, máu, sát thương, phản đạn.

- `game.hpp`  và  `game.cpp`:
  + Chứa `main()` – hàm khởi tạo game, chạy vòng lặp chính, vẽ, xử lý sự kiện, kết thúc game.
 
# 5. Về đồ họa:
- Tất cả hình ảnh, biểu tượng, animation và hiệu ứng hình ảnh trong trò chơi được lấy từ các nguồn miễn phí tại OpenGameArt.org — đảm bảo vừa chất lượng vừa hợp lệ về bản quyền.

# 🔽 Download & Play

👉 [Click here to download the game (Windows)](https://github.com/ledinhdung2604/Aliens-Resistance-War/releases/download/v1.0/Aliens-Resistance-War.zip)

- Đường link tải game trên bao gồm cả source code và file chạy game.

- Xử lí sau khi tải game từ đường link trên:
1. Giải nén file `.zip`
2. Chạy `main.exe`
3. Play!
