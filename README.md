🌌 Aliens: Resistance War
Aliens: Resistance War là một tựa game bắn tàu vũ trụ không gian 2D được phát triển bằng thư viện SDL2. Người chơi sẽ vào vai một phi thuyền đơn độc chiến đấu chống lại các đợt tấn công của người ngoài hành tinh, thiên thạch, boss khổng lồ và các thế lực vũ trụ bí ẩn như hố đen.

![Gameplay Preview](image/ARWBackground.png)

1. Gameplay:
- Bạn là phi công cuối cùng điều khiển tàu không gian để chiến đấu và sinh tồn
- Bắn hạ enemy và boss để tăng điểm và nhận buff hỗ trợ.
- Đạn địch, thiên thạch, hố đen và những đòn đánh từ boss sẽ khá khó tránh, đòi hỏi phản xạ tốt.
- Trò chơi không có hồi kết, mục tiêu của bạn là đạt điểm càng cao càng tốt.

2. Các cơ chế chính:
- Di chuyển & Bắn: Người chơi điều khiển phi thuyền bằng các phím (A, S, D, W) hoặc các phím mũi tên và có thể bắn bằng phím Space với tốc độ di chuyển, tốc độ và sát thương đạn tùy theo buff đang hoạt động.
- Kẻ thù: Các loại enemy di chuyển theo các quỹ đạo khác nhau và tấn công bằng đạn. Càng về sau, game sẽ có boss xuất hiện với kỹ năng và cách tấn công đặc biệt.
- Boss Fight: Boss xuất hiện định kỳ sau mỗi 30–60 giây và có thanh máu riêng trên màn hình. Có bốn loại boss với các phương thức tấn công khác nhau:
  + Boss 1: bắn loạt đạn tỏa ra các hướng với giới hạn góc 180 độ về phía dưới.
  + Boss 2: bắn đạn lớn về phía player, gây sát thương cao.
  + Boss 3: bắn 3 viên đạn đuổi player trong khoảng thời gian ngắn.
  + Boss 4: bắn đạn nảy bốn lần rồi mới ra khỏi màn hình.
- Meteor & Black Hole: Thiên thạch rơi ngẫu nhiên gây sát thương và hố đen xuất hiện định kỳ làm chậm người chơi, gây khó khăn trong di chuyển.
- Buff hệ thống: Tiêu diệt kẻ thù có thể rơi ra buff hỗ trợ với các hiệu ứng như:
  + HEAL: Hồi máu
  + SHIELD: Khiên bảo vệ
  + DAMAGE BOOST: Tăng sát thương
  + ATTACK SPEED: Tăng tốc độ bắn
  + UP MAX HP: Tăng máu tối đa
  + REFLECT: Phản lại đạn địch
  + FREEZE: Đóng băng enemy
  + SPEED BOOST: Tăng tốc di chuyển
- Khi player bị tiêu diệt, người chơi có thể chọn R để tiếp tục hoặc Q để thoát.

3.  Tính năng khác
- Menu mở đầu với ảnh nền và nút "Play".
- Thanh máu động cho cả Player và Boss.
- Hiệu ứng lửa động phía dưới phi thuyền.
- Hiển thị điểm số và buff đang kích hoạt (+5 điểm khi tiêu diệt enemy, +20 điểm khi tiêu diệt boss).
- Xử lý va chạm thông minh cho đạn, địch, buff, thiên thạch, hố đen...
- Hiệu ứng kết thúc Game Over với điểm số và hướng dẫn thao tác tiếp theo.
