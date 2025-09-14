## 5.1. Nguồn gốc của Kiến trúc Máy tính Hiện đại

Khi lần theo nguồn gốc của kiến trúc máy tính hiện đại, ta dễ bị cuốn vào suy nghĩ rằng các máy tính hiện đại là một chuỗi tiến hóa tuyến tính, trong đó mỗi máy đơn giản là một phiên bản cải tiến của máy tiền nhiệm. Mặc dù cách nhìn nhận này có thể đúng với một số lớp kiến trúc nhất định (hãy nghĩ đến các cải tiến tuần tự của iPhone X từ chiếc iPhone đầu tiên), thì gốc rễ của cây kiến trúc lại không rõ ràng như vậy.

Từ những năm 1700 đến đầu những năm 1900, các nhà toán học chính là những *máy tính con người* đầu tiên, thực hiện các phép tính phục vụ cho các ứng dụng khoa học và kỹ thuật¹. Từ “computer” ban đầu dùng để chỉ “người tính toán”. Các nữ toán học thường đảm nhiệm vai trò này. Thực tế, việc sử dụng phụ nữ làm máy tính con người phổ biến đến mức độ phức tạp tính toán từng được đo bằng đơn vị “kilo-girls”, tức là khối lượng công việc mà một nghìn máy tính con người có thể hoàn thành trong một giờ². Phụ nữ được xem là giỏi hơn nam giới trong việc thực hiện các phép toán, vì họ thường làm việc có phương pháp hơn. Tuy nhiên, họ không được phép giữ chức danh kỹ sư, và do đó bị phân công vào các công việc “tầm thường” hơn như thực hiện các phép tính phức tạp.

Chiếc máy tính số đa dụng đầu tiên, *Analytical Engine*, được thiết kế bởi nhà toán học người Anh Charles Babbage, người được một số người xem là cha đẻ của máy tính. Analytical Engine là phần mở rộng của phát minh ban đầu của ông – Difference Engine – một máy tính cơ học có khả năng tính toán các hàm đa thức. Ada Lovelace, người có lẽ nên được gọi là mẹ của ngành điện toán, là người đầu tiên phát triển chương trình máy tính và công bố thuật toán có thể được tính toán bằng Analytical Engine của Babbage. Trong ghi chú của bà có đoạn thể hiện sự nhận thức về tính chất đa dụng của Analytical Engine: “\[t\]he Analytical Engine has no pretensions whatever to originate anything. It can do whatever we know how to order it to perform.”³ Tuy nhiên, khác với máy tính hiện đại, Analytical Engine là một thiết bị cơ học và chỉ được xây dựng một phần. Hầu hết các nhà thiết kế của những cỗ máy tiền thân trực tiếp của máy tính hiện đại đều không biết đến công trình của Babbage và Lovelace khi họ phát triển các thiết bị của riêng mình.

Do đó, có lẽ chính xác hơn khi xem kiến trúc máy tính hiện đại như được hình thành từ “nồi súp nguyên thủy” của các ý tưởng và đổi mới xuất hiện vào những năm 1930 và 1940. Ví dụ, năm 1937, Claude Shannon – một sinh viên tại MIT – đã viết luận văn thạc sĩ được xem là có ảnh hưởng nhất mọi thời đại. Dựa trên công trình của George Boole (nhà toán học đã phát triển đại số Boolean), Shannon chứng minh rằng logic Boolean có thể được áp dụng vào mạch điện và dùng để phát triển các công tắc điện. Điều này dẫn đến sự ra đời của hệ thống tính toán nhị phân và phần lớn thiết kế mạch số sau này. Trong khi nam giới thiết kế nhiều máy tính điện tử đầu tiên, phụ nữ (vốn không được phép làm kỹ sư) lại trở thành những người tiên phong trong lập trình, dẫn đầu việc thiết kế và phát triển nhiều đổi mới phần mềm ban đầu như ngôn ngữ lập trình, trình biên dịch, thuật toán và hệ điều hành.

Việc thảo luận toàn diện về sự hình thành của kiến trúc máy tính nằm ngoài phạm vi của cuốn sách này (xem *Turing’s Cathedral*⁴ của George Dyson và *The Innovators*⁶ của Walter Isaacson để có cái nhìn chi tiết hơn); tuy nhiên, chúng ta sẽ điểm qua một số đổi mới quan trọng trong thập niên 1930 và 1940 đã góp phần hình thành kiến trúc máy tính hiện đại.

---

### 5.1.1. Máy Turing

Năm 1937, nhà toán học người Anh Alan Turing đã đề xuất⁷ “Logical Computing Machine” – một máy tính lý thuyết. Turing sử dụng cỗ máy này để chứng minh rằng không tồn tại lời giải cho bài toán quyết định (tiếng Đức: *Entscheidungsproblem*) do các nhà toán học David Hilbert và Wilhelm Ackermann đặt ra vào năm 1928. Bài toán quyết định là một thuật toán nhận đầu vào là một mệnh đề và xác định xem mệnh đề đó có đúng với mọi trường hợp hay không. Turing chứng minh rằng không tồn tại thuật toán như vậy bằng cách chỉ ra rằng *halting problem* (liệu máy *X* có dừng lại với đầu vào *y* hay không?) là không thể quyết định được đối với máy của ông. Trong quá trình chứng minh, Turing mô tả một cỗ máy phổ quát có khả năng thực hiện các tác vụ của bất kỳ máy tính nào khác. Alonzo Church – giáo sư hướng dẫn luận văn của Turing tại Đại học Princeton – là người đầu tiên gọi “logical computing machine” là *Turing machine*, và phiên bản phổ quát của nó là *universal Turing machine*.

Sau đó, Turing trở về Anh và phục vụ đất nước trong đơn vị giải mã tại Bletchley Park trong Thế chiến II. Ông đóng vai trò quan trọng trong việc thiết kế và chế tạo *Bombe* – một thiết bị điện cơ giúp phá mã do máy Enigma tạo ra, vốn được Đức Quốc xã sử dụng phổ biến để bảo vệ thông tin liên lạc nhạy cảm trong chiến tranh.

Sau chiến tranh, Turing thiết kế *automatic computing engine* (ACE). ACE là một máy tính lưu trữ chương trình, nghĩa là cả lệnh chương trình và dữ liệu đều được nạp vào bộ nhớ máy tính và được thực thi bởi máy tính đa dụng. Bài báo của ông, công bố năm 1946, có lẽ là mô tả chi tiết nhất về một máy tính như vậy⁸.

---

### 5.1.2. Các Máy Tính Điện Tử Đầu Tiên

Thế chiến II đã thúc đẩy mạnh mẽ sự phát triển của các máy tính đầu tiên. Tuy nhiên, do tính chất tuyệt mật của các hoạt động quân sự trong chiến tranh, nhiều chi tiết về các đổi mới diễn ra trong thời kỳ sôi động này không được công bố rộng rãi cho đến nhiều năm sau. Một ví dụ điển hình là Colossus – một cỗ máy do kỹ sư người Anh Tommy Flowers thiết kế để giúp phá mã Lorenz, được Đức Quốc xã dùng để mã hóa thông tin tình báo cấp cao. Một phần công trình của Alan Turing đã hỗ trợ thiết kế Colossus. Được xây dựng năm 1943, Colossus được xem là máy tính đầu tiên có khả năng lập trình, kỹ thuật số và hoàn toàn điện tử. Tuy nhiên, nó là máy tính chuyên dụng, được thiết kế riêng cho mục đích giải mã. Lực lượng Nữ Hải quân Hoàng gia Anh (WRNS, gọi là “Wrens”) đảm nhiệm vai trò vận hành Colossus. Mặc dù *General Report of the Tunny*¹⁴ ghi nhận rằng một số thành viên Wrens có năng lực trong công việc mật mã, nhưng không ai trong số họ được trao chức danh chuyên gia mật mã, mà chỉ được giao các nhiệm vụ vận hành Colossus mang tính lặp lại¹⁵.

Ở bên kia Đại Tây Dương, các nhà khoa học và kỹ sư Mỹ cũng đang miệt mài phát triển máy tính của riêng mình. Giáo sư Harvard Howard Aiken (đồng thời là sĩ quan Hải quân Dự bị Hoa Kỳ) đã thiết kế Mark I – một máy tính điện cơ, đa dụng, có khả năng lập trình. Được xây dựng năm 1944, nó hỗ trợ việc thiết kế bom nguyên tử. Aiken phát triển máy tính của mình gần như không biết đến công trình của Turing, và được thúc đẩy bởi mục tiêu hiện thực hóa chiếc Analytical Engine của Charles Babbage⁶. Một đặc điểm quan trọng của Mark I là nó hoàn toàn tự động và có thể chạy liên tục trong nhiều ngày mà không cần can thiệp của con người⁶ – đây sẽ là một đặc điểm nền tảng trong thiết kế máy tính tương lai.

Trong khi đó, các kỹ sư Mỹ John Mauchly và Presper Eckert tại Đại học Pennsylvania đã thiết kế và xây dựng *Electronic Numerical Integrator and Computer* (ENIAC) vào năm 1945. ENIAC được xem là tiền thân của máy tính hiện đại. Nó là máy tính kỹ thuật số (dù sử dụng hệ thập phân thay vì nhị phân), hoàn toàn điện tử, có khả năng lập trình và đa dụng. Mặc dù phiên bản ban đầu của ENIAC chưa có khả năng lưu trữ chương trình, tính năng này đã được tích hợp vào máy trước khi thập kỷ kết thúc. ENIAC được tài trợ và xây dựng cho Phòng Thí nghiệm Nghiên cứu Đạn đạo của Quân đội Hoa Kỳ, và được thiết kế chủ yếu để tính toán quỹ đạo đạn đạo. Sau này, nó còn được sử dụng để hỗ trợ thiết kế bom nhiệt hạch.

Dưới đây là bản dịch hoàn chỉnh của phần văn bản bạn cung cấp, tuân thủ đầy đủ các quy tắc dịch thuật kỹ thuật và cú pháp footnote Markdown như bạn yêu cầu:

---

Khi nam giới được điều động vào quân đội trong Thế chiến II, phụ nữ được tuyển dụng để hỗ trợ nỗ lực chiến tranh với vai trò là các máy tính con người. Khi những máy tính điện tử đầu tiên xuất hiện, phụ nữ trở thành những lập trình viên đầu tiên, vì công việc lập trình lúc đó được xem là công việc thư ký. Không có gì ngạc nhiên khi nhiều đổi mới ban đầu trong lập trình – như trình biên dịch đầu tiên, khái niệm phân chia chương trình thành mô-đun, kỹ thuật gỡ lỗi (debugging), và ngôn ngữ assembly – đều được ghi nhận là do phụ nữ sáng tạo. Ví dụ, Grace Hopper đã phát triển ngôn ngữ lập trình cấp cao đầu tiên, độc lập với phần cứng (COBOL), cùng với trình biên dịch của nó. Bà cũng là lập trình viên cho máy Mark I và là tác giả cuốn sách mô tả cách vận hành của nó.

Những lập trình viên của ENIAC gồm sáu phụ nữ: Jean Jennings Bartik, Betty Snyder Holberton, Kay McNulty Mauchly, Frances Bilas Spence, Marlyn Wescoff Meltzer, và Ruth Lichterman Teitelbaum. Khác với các Wrens, nhóm phụ nữ ENIAC được trao quyền tự chủ rất lớn trong công việc; họ chỉ được cung cấp sơ đồ dây nối của ENIAC và được yêu cầu tự tìm hiểu cách hoạt động và cách lập trình nó. Ngoài việc sáng tạo ra cách lập trình (và gỡ lỗi) cho một trong những máy tính điện tử đa dụng đầu tiên trên thế giới, các lập trình viên ENIAC còn phát triển khái niệm sơ đồ luồng thuật toán (algorithmic flow chart), và các khái niệm lập trình quan trọng như thủ tục con (subroutine) và lồng ghép (nesting). Giống như Grace Hopper, Jean Jennings Bartik và Betty Snyder Holberton sau này đều có sự nghiệp lâu dài trong ngành điện toán, và là những người tiên phong trong lĩnh vực này. Đáng tiếc là mức độ đóng góp của phụ nữ trong giai đoạn đầu của ngành điện toán vẫn chưa được ghi nhận đầy đủ. Do không thể thăng tiến, nhiều phụ nữ đã rời khỏi lĩnh vực này sau Thế chiến II. Để tìm hiểu thêm về các lập trình viên nữ thời kỳ đầu, bạn đọc có thể tham khảo *Recoding Gender* của Janet Abbate[^11], phim tài liệu *Top Secret Rosies* do PBS sản xuất[^12] do LeAnn Erickson đạo diễn, và *The Computers* của Kathy Kleiman[^13].

Không chỉ Anh và Mỹ quan tâm đến tiềm năng của máy tính. Tại Đức, Konrad Zuse đã phát triển máy tính số điện cơ đa dụng đầu tiên có khả năng lập trình – Z3 – hoàn thành vào năm 1941. Zuse tự mình thiết kế máy này mà không hề biết đến công trình của Turing hay các nhà khoa học khác. Đáng chú ý, thiết kế của Zuse sử dụng hệ nhị phân (thay vì thập phân), là máy tính đầu tiên thuộc loại này dùng hệ nhị phân. Tuy nhiên, Z3 đã bị phá hủy trong một cuộc không kích vào Berlin, và Zuse không thể tiếp tục công việc cho đến năm 1950. Công trình của ông phần lớn không được công nhận cho đến nhiều năm sau. Ngày nay, ông được xem là cha đẻ của ngành điện toán tại Đức.

---

### 5.1.3. Vậy von Neumann Biết Những Gì?

Từ phần thảo luận về nguồn gốc của kiến trúc máy tính hiện đại, có thể thấy rằng trong thập niên 1930 và 1940 đã có nhiều đổi mới dẫn đến sự ra đời của máy tính như chúng ta biết ngày nay. Năm 1945, John von Neumann công bố bài viết *First draft of a report on the EDVAC*[^9], mô tả một kiến trúc làm nền tảng cho các máy tính hiện đại. EDVAC là hậu duệ của ENIAC. Nó khác ENIAC ở chỗ là máy tính nhị phân thay vì thập phân, và là máy tính lưu trữ chương trình. Ngày nay, mô tả kiến trúc của EDVAC trong bài viết đó được gọi là *von Neumann architecture*.

**Von Neumann architecture** mô tả một máy tính đa dụng – được thiết kế để chạy bất kỳ chương trình nào. Nó cũng sử dụng mô hình lưu trữ chương trình, nghĩa là cả lệnh chương trình và dữ liệu đều được nạp vào máy tính để thực thi. Trong mô hình von Neumann, không có sự phân biệt giữa lệnh và dữ liệu; cả hai đều được nạp vào bộ nhớ trong của máy tính, và lệnh chương trình được truy xuất từ bộ nhớ và thực thi bởi các đơn vị chức năng của máy tính trên dữ liệu chương trình.

Đóng góp của John von Neumann đan xen với nhiều câu chuyện trước đó trong lịch sử điện toán. Là một nhà toán học người Hungary, ông từng là giáo sư tại Viện Nghiên cứu Cao cấp và Đại học Princeton, và từng là người cố vấn ban đầu cho Alan Turing. Sau này, von Neumann trở thành nhà khoa học nghiên cứu trong Dự án Manhattan, điều này đưa ông đến với Howard Aiken và máy Mark I; ông cũng từng là cố vấn cho dự án ENIAC, và thường xuyên trao đổi thư từ với Eckert và Mauchly. Bài viết nổi tiếng mô tả EDVAC của ông xuất phát từ công trình về *Electronic Discrete Variable Automatic Computer* (EDVAC), được Eckert và Mauchly đề xuất với Quân đội Hoa Kỳ, và được xây dựng tại Đại học Pennsylvania. EDVAC bao gồm nhiều đổi mới trong thiết kế kiến trúc, tạo nền tảng cho hầu hết các máy tính hiện đại: nó là máy đa dụng, sử dụng hệ số nhị phân, có bộ nhớ trong, và hoàn toàn điện tử. Phần lớn vì von Neumann là tác giả duy nhất của bài viết[^9], nên thiết kế kiến trúc được mô tả trong đó chủ yếu được ghi nhận cho ông và được gọi là kiến trúc von Neumann. Cần lưu ý rằng Turing đã mô tả rất chi tiết thiết kế của một máy tương tự vào năm 1946. Tuy nhiên, vì bài viết của von Neumann được công bố trước bài của Turing, nên ông được ghi nhận chính cho những đổi mới này.

Dù ai là người “thật sự” phát minh ra kiến trúc von Neumann, thì đóng góp của von Neumann vẫn không thể phủ nhận. Ông là một nhà toán học và nhà khoa học xuất chúng. Các đóng góp của ông trong toán học trải rộng từ lý thuyết tập hợp đến cơ học lượng tử và lý thuyết trò chơi. Trong lĩnh vực điện toán, ông cũng được xem là người phát minh ra thuật toán *merge sort*. Walter Isaacson, trong cuốn *The Innovators*, cho rằng một trong những thế mạnh lớn nhất của von Neumann là khả năng hợp tác rộng rãi và trực giác nhạy bén với các khái niệm mới[^6]. Nhiều nhà thiết kế máy tính thời kỳ đầu làm việc khá biệt lập. Isaacson lập luận rằng khi chứng kiến sự chậm chạp của máy Mark I, von Neumann đã trực giác nhận ra giá trị của một máy tính điện tử thực sự, và sự cần thiết của việc lưu trữ và sửa đổi chương trình trong bộ nhớ. Do đó, có thể cho rằng von Neumann – thậm chí hơn cả Eckert và Mauchly – đã nắm bắt và đánh giá đầy đủ sức mạnh của một máy tính điện tử lưu trữ chương trình[^6].


[^1]: Grier, David Alan. *When Computers Were Human*. Princeton University Press, 2005.  
[^2]: Light, Jennifer S. *When Computers Were Women*. Technology and Culture, Vol. 40, No. 3 (1999), pp. 455–483.  
[^3]: Lovelace, Ada. *Notes by the Translator*. In Menabrea, Luigi Federico. *Sketch of the Analytical Engine Invented by Charles Babbage*. 1842.  
[^4]: Dyson, George. *Turing’s Cathedral: The Origins of the Digital Universe*. Pantheon Books, 2012.  
[^5]: B. Jack Copeland et al., *Colossus: The Secrets of Bletchley Park’s Codebreaking Computers*. Oxford University Press, 2006.  
[^6]: Isaacson, Walter. *The Innovators: How a Group of Hackers, Geniuses, and Geeks Created the Digital Revolution*. Simon & Schuster, 2014.  
[^7]: Turing, Alan. *On Computable Numbers, with an Application to the Entscheidungsproblem*. Proceedings of the London Mathematical Society, 1936.  
[^8]: Turing, Alan. *Proposed Electronic Calculator*. National Physical Laboratory Report, 1946.  
[^9]: von Neumann, John. *First Draft of a Report on the EDVAC*. 1945.  
[^11]: Abbate, Janet. *Recoding Gender: Women’s Changing Participation in Computing*. MIT Press, 2012.  
[^12]: Erickson, LeAnn. *Top Secret Rosies: The Female Computers of WWII*. PBS Documentary, 2010.  
[^13]: Kleiman, Kathy. *The Computers*. Documentary, 2016.  
[^14]: *General Report on Tunny*, British Government Code and Cypher School, 1945.  
[^15]: Hicks, Marie. *Programmed Inequality: How Britain Discarded Women Technologists and Lost Its Edge in Com

### 5.1.4. References 


1.  David Alan Grier, *\"When Computers Were Human\"*, Princeton
    University Press, 2005.

2.  Megan Garber, *\"Computing Power Used to be Measured in
    \'Kilo-Girls\'\"*. The Atlantic, October 16, 2013.
    [https://www.theatlantic.com/technology/archive/2013/10/computing-power-used-to-be-measured-in-kilo-girls/280633/](https://www.theatlantic.com/technology/archive/2013/10/computing-power-used-to-be-measured-in-kilo-girls/280633/){.bare}

3.  Betty Alexandra Toole, *\"Ada, The Enchantress of Numbers\"*.
    Strawberry Press, 1998.

4.  George Dyson, *Turing's Cathedral: the origins of the digital
    universe*. Pantheon. 2012.

5.  Jack Copeland, *\"Colossus: The Secrets of Bletchley Park's
    Code-breaking Computers\"*.

6.  Walter Isaacson. *\"The Innovators: How a group of inventors,
    hackers, genius and geeks created the digital revolution\"*. Simon
    and Schuster. 2014.

7.  Alan M. Turing. *\"On computable numbers, with an application to the
    Entscheidungsproblem\"*. *Proceedings of the London mathematical
    society* 2(1). pp. 230---​265. 1937.

8.  Brian Carpenter and Robert Doran. *\"The other Turing Machine\"*.
    *The Computer Journal* 20(3) pp. 269---​279. 1977.

9.  John von Neumann. *\"First Draft of a Report on the EDVAC (1945)\"*.
    Reprinted in *IEEE Annals of the history of computing* 4. pp.
    27---​75. 1993.

10. Arthur Burks, Herman Goldstine, John von Neumann. *\"Preliminary
    discussion of the logical design of an electronic computing
    instrument (1946)\"*. Reprinted by *The Origins of Digital
    Computers* (Springer), pp. 399---​413. 1982.

11. Janet Abbate. *\"Recoding gender: Women's changing participation in
    computing\"*. MIT Press. 2012.

12. LeAnn Erickson. *\"Top Secret Rosies: The Female Computers of World
    War II\"*. Public Broadcasting System. 2010.

13. Kathy Kleiman, *\"The Computers\"*.
    [http://eniacprogrammers.org/](http://eniacprogrammers.org/){.bare}

14. *\"Breaking Teleprinter Ciphers at Bletchley Park: An edition of
    I.J. Good, D. Michie and G. Timms: General Report on Tunny with
    Emphasis on Statistical Methods (1945)\"*. Editors: Reeds, Diffie,
    Fields. Wiley, 2015.

15. Janet Abbate, *\"Recoding Gender\"*, MIT Press, 2012.






