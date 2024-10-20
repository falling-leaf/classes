<template>
    <el-scrollbar height="100%" style="width: 100%;">
        <!-- 标题和搜索框 -->
        <div style="margin-top: 20px; margin-left: 40px; font-size: 2em; font-weight: bold; ">图书管理
            <el-button type="info" @click="Querybooks" style="margin-left: 30px; margin-right: 30px; float: right;" clearable round>|——取消查询——|</el-button>
            <el-button type="warning" @click="this.searchbookVisible = true" style="margin-left: 30px; margin-right: 30px; float: right;" clearable round>|——查询图书——|</el-button>
            <el-button type="primary" @click="this.storebookVisible = true" style="margin-left: 30px; margin-right: 30px; float: right;" clearable round>|——批量入库——|</el-button>
        </div>

        <!-- 图书卡片显示区 -->
        <div style="display: flex;flex-wrap: wrap; justify-content: start;">

            <!-- 图书卡片 -->
            <div class="bookBox" v-for="book in books" :key="book.id">
                <div>
                    <!-- 卡片标题 -->
                    <div style="font-size: 25px; font-weight: bold;">No. {{ book.id }}</div>

                    <el-divider />

                    <!-- 卡片内容 -->
                    <div style="margin-left: 10px; text-align: start; font-size: 16px;">
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">标题：</span>{{ book.title }}</p>
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">类别：</span>{{ book.category }}</p>
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">作者：</span>{{ book.author }}</p>
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">出版社：</span>{{ book.press }}</p>
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">出版年份：</span>{{ book.publishYear }}</p>
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">价格：</span>{{ book.price }}</p>
                        <p style="padding: 2.5px;"><span style="font-weight: bold;">库存：</span>{{ book.stock }} <el-button type="primary" :icon="Edit" circle
                            @click= "this.changestockVisible = true, this.changestockinfo.bookID = book.id" style="margin-left: 15px;" /></p>
                    </div>

                    <el-divider />

                    <!-- 卡片操作 -->
                    <div style="margin-top: 10px;">
                        <el-button type="primary" @click="this.toModifyInfo.id = book.id, this.toModifyInfo.title = book.title,
                this.toModifyInfo.category = book.category, this.toModifyInfo.press = book.press, this.toModifyInfo.price = book.price,
                this.toModifyInfo.author = book.author, this.toModifyInfo.publishYear = book.publishYear,
                this.modifybookVisible = true" round>|——修改图书信息——|</el-button>
                    </div>
                    <div style="margin-top: 10px;">
                        <el-button type="success"
                            @click="this.toBorrow = this.toBorrowInfo.bookID = book.id, this.toBorrowInfo.cardID = '', this.borrowbookVisible = true"
                            round>|——凭卡借阅图书——|</el-button>
                    </div>
                    <div style="margin-top: 10px;">
                        <el-button type="danger"
                            @click="this.toRemove = book.id, this.removebookVisible = true"
                            round>|——删除图书信息——|</el-button>
                    </div>
                </div>
            </div>

            <!-- 新建图书卡片 -->
            <el-button class="newbookBox"
                @click="newbookInfo.title = '', newbookInfo.category = '', newbookInfo.author = '', newbookInfo.press = '', newbookInfo.publishYear = '', newbookInfo.price = '', newbookInfo.stock = '', newbookVisible = true">
                <el-icon style="height: 50px; width: 50px;">
                    <Plus style="height: 100%; width: 100%;" />
                </el-icon>
            </el-button>

        </div>


        <!-- 新建图书对话框 -->
        <el-dialog v-model="newbookVisible" title="注册新图书" width="30%" align-center>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                标题：
                <el-input v-model="newbookInfo.title" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                类别：
                <el-input v-model="newbookInfo.category" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                作者：
                <el-input v-model="newbookInfo.author" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版社：
                <el-input v-model="newbookInfo.press" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版年份：
                <el-input v-model="newbookInfo.publishYear" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                价格：
                <el-input v-model="newbookInfo.price" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                库存：
                <el-input v-model="newbookInfo.stock" style="width: 12.5vw;" clearable />
            </div>

            <template #footer>
                <span>
                    <el-button @click="newbookVisible = false">取消</el-button>
                    <el-button type="primary" @click="ConfirmNewbook"
                        :disabled="newbookInfo.title.length === 0 || newbookInfo.category.length === 0 || newbookInfo.press.length === 0 || newbookInfo.publishYear.length === 0 || newbookInfo.author.length === 0 || newbookInfo.price.length === 0 || newbookInfo.stock.length === 0">确定</el-button>
                </span>
            </template>
        </el-dialog>


        <!-- 修改信息对话框 -->   
        <el-dialog v-model="modifybookVisible" :title="'修改图书信息(图书ID: ' + this.toModifyInfo.id + ')'" width="30%"
            align-center>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                标题：
                <el-input v-model="toModifyInfo.title" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                类别：
                <el-input v-model="toModifyInfo.category" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                作者：
                <el-input v-model="toModifyInfo.author" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版社：
                <el-input v-model="toModifyInfo.press" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版年份：
                <el-input v-model="toModifyInfo.publishYear" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                价格：
                <el-input v-model="toModifyInfo.price" style="width: 12.5vw;" clearable />
            </div>

            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="modifybookVisible = false">取消</el-button>
                    <el-button type="primary" @click="ConfirmModifybook"
                        :disabled="toModifyInfo.title.length === 0 || toModifyInfo.category.length === 0 || toModifyInfo.press.length === 0 || toModifyInfo.publishYear.length === 0 || toModifyInfo.author.length === 0 || toModifyInfo.price.length === 0">确定</el-button>
                </span>
            </template>
        </el-dialog>

        <!-- 删除图书对话框 -->  
        <el-dialog v-model="removebookVisible" title="删除图书信息" width="30%">
            <span>确定删除<span style="font-weight: bold;">{{ toRemove }}号图书</span>吗？</span>

            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="removebookVisible = false">取消</el-button>
                    <el-button type="danger" @click="ConfirmRemovebook">
                        删除
                    </el-button>
                </span>
            </template>
        </el-dialog>

        <!-- 借阅图书对话框 -->  
        <el-dialog v-model="borrowbookVisible" :title="'凭卡借阅图书(图书ID: ' + this.toBorrowInfo.bookID + ')'" width="30%">
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                借书证ID
                <el-input v-model="toBorrowInfo.cardID" style="width: 12.5vw;" clearable />
            </div>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="borrowbookVisible = false">取消</el-button>
                    <el-button type="primary" @click="ConfirmBorrowbook"
                        :disabled="toBorrowInfo.cardID.length === 0">确定</el-button>
                </span>
            </template>
        </el-dialog>

        <!-- 查询图书对话框 -->  
        <el-dialog v-model="searchbookVisible" :title="'查询图书(查询完成后，请点击主界面[取消查询]以恢复!)'" width="30%">
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                图书类别（精确查找）
                <el-input v-model="toSearch.category" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                书名
                <el-input v-model="toSearch.title" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版社
                <el-input v-model="toSearch.press" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                作者
                <el-input v-model="toSearch.author" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版年份下限
                <el-input v-model="toSearch.publishYearmin" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                出版年份上限
                <el-input v-model="toSearch.publishYearmax" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                价格下限
                <el-input v-model="toSearch.pricemin" style="width: 12.5vw;" clearable />
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                价格上限
                <el-input v-model="toSearch.pricemax" style="width: 12.5vw;" clearable />
            </div>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="searchbookVisible = false">取消</el-button>
                    <el-button type="primary" @click="ConfirmSearchbook">确定</el-button>
                </span>
            </template>
        </el-dialog>

        <!-- 批量入库对话框 -->  
        <el-dialog v-model="storebookVisible" :title="'批量入库（文件路径请填写绝对路径）'" width="30%">
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                文件路径（绝对路径）
                <el-input v-model="storebookPath" style="width: 12.5vw;" clearable />
            </div>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="storebookVisible = false">取消</el-button>
                    <el-button type="primary" @click="ConfirmStorebook"
                        :disabled="storebookPath.length === 0">确定</el-button>
                </span>
            </template>
        </el-dialog>

        <!-- 增减库存对话框 -->  
        <el-dialog v-model="changestockVisible" :title="'更改库存(请输入库存的变动数目，如+3,-5等)'" width="30%">
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                图书ID: {{ this.changestockinfo.bookID }}
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                库存变动数目
                <el-input v-model="changestockinfo.stock" style="width: 12.5vw;" clearable />
            </div>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="changestockVisible = false">取消</el-button>
                    <el-button type="primary" @click="ConfirmChangestock"
                        :disabled="changestockinfo.stock.length === 0">确定</el-button>
                </span>
            </template>
        </el-dialog>


    </el-scrollbar>

</template>

<script>
import { Delete, Edit, Search } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'
import axios from 'axios'

export default {
    data() {
        return {
            books: [{ // 图书列表
                id: 1,
                category: '科学',
                title: 'Java:从入门到入土',
                press: '2024-1-1',
                publishYear: 2024,
                author: 'why',
                price: 520.1,
                stock: 100
            }, {
                id: 2,
                category: '哲学',
                title: 'live or death',
                press: '2024-1-1',
                publishYear: 2024,
                author: 'why',
                price: 521.1,
                stock: 0
            }
            ],
            Delete,
            Edit,
            Search,
            searchbookVisible: false,
            toSearch: {
                category: '',
                title: '',
                press: '',
                author: '',
                publishYearmin: '',
                publishYearmax: '',
                pricemin: '',
                pricemax: ''
            }, 
            newbookVisible: false, // 新建图书对话框可见性
            removebookVisible: false, // 删除图书对话框可见性
            toRemove: 0, // 待删除图书号
            newbookInfo: { // 待新建图书信息
                category: '',
                title: '',
                press: '',
                publishYear: 0,
                author: '',
                price: 0.0,
                stock: 0
            },
            modifybookVisible: false, // 修改信息对话框可见性
            toModifyInfo: { // 待修改图书信息
                id: 0,
                category: '',
                title: '',
                press: '',
                publishYear: '',
                author: '',
                price: ''
            },
            toBorrow: 0,
            borrowbookVisible: false,
            toBorrowInfo: {
                cardID: 0,
                bookID: 0
            },
            storebookVisible: false,
            storebookPath: '',
            tochangestock: 0,
            changestockVisible: false,
            changestockinfo: {
                bookID: '',
                stock: ''
            }
        }
    },
    methods: {
        ConfirmStorebook() {
            axios.post("/book",
                {
                    path: this.storebookPath
                })
                .then(response => {
                    if (response.data === "invalid path"){
                        ElMessage.error("路径不存在或读取文件失败")
                    } else if (response.data === "store failed"){
                        ElMessage.error("注册图书失败，有书已经存在于书库中")
                    } else {
                        ElMessage.success("注册图书成功！")
                        this.storebookVisible = false
                        this.Querybooks()
                    }
                })
        },
        ConfirmNewbook() {
            axios.post("/book",
                { 
                    category: this.newbookInfo.category,
                    title: this.newbookInfo.title,
                    press: this.newbookInfo.press,
                    publishYear: this.newbookInfo.publishYear,
                    author: this.newbookInfo.author,
                    price: this.newbookInfo.price,
                    stock: this.newbookInfo.stock
                })
                .then(response => {
                    if (response.data === "number constrain"){
                        ElMessage.error("图书注册失败，请注意出版年份/价格/库存必须为数字~")
                    }
                    else if (response.data === "book existed"){
                        ElMessage.error("图书注册失败，该书已经存在~") // 显示消息提醒
                        this.newbookVisible = false // 将对话框设置为不可见
                        this.Querybooks() // 重新查询借书证以刷新页面
                    }
                    else {
                        ElMessage.success("图书注册成功~") // 显示消息提醒
                        this.newbookVisible = false // 将对话框设置为不可见
                        this.Querybooks() // 重新查询借书证以刷新页面
                    }
                })
        },
        ConfirmBorrowbook() {
            axios.post("/book",
                {
                    bookID: this.toBorrowInfo.bookID,
                    cardID: this.toBorrowInfo.cardID
                })
                .then(response =>{
                    if (response.data === "borrow failed"){
                        ElMessage.error("图书借阅失败，库存为0或已借阅的书无法被借阅~")
                        this.borrowbookVisible = false;
                    }
                    else {
                        ElMessage.success("图书借阅成功~") // 显示消息提醒
                        this.borrowbookVisible = false // 将对话框设置为不可见
                        this.Querybooks() // 重新查询借书证以刷新页面
                    }
                })
        },
        ConfirmChangestock() {
            axios.put("/book",
                {
                    bookID: this.changestockinfo.bookID,
                    stock: this.changestockinfo.stock
                })
                .then(response =>{
                    if (response.data === "number constrain"){
                        ElMessage.error("库存修改失败，请保证库存修改量为整数")
                    } else if (response.data === "changing stock failed"){
                        ElMessage.error("库存修改失败，注意库存数量最终不能为负数！")
                    } else if (response.data === "stock changed successfully"){
                        ElMessage.success("库存修改成功")
                        this.changestockVisible = false
                        this.Querybooks()
                    }
                })
        },
        ConfirmModifybook() {
            axios.put("/book",
                {
                    id: this.toModifyInfo.id,
                    category: this.toModifyInfo.category,
                    title: this.toModifyInfo.title,
                    press: this.toModifyInfo.press,
                    publishYear: this.toModifyInfo.publishYear,
                    author: this.toModifyInfo.author,
                    price: this.toModifyInfo.price
                }
            )
                .then(response =>{
                    if (response.data === "number constrain"){
                        ElMessage.error("图书修改失败，请注意出版年份/价格必须为数字~")
                    }
                    else if (response.data === "modifying failed"){
                        ElMessage.error("图书修改失败") // 显示消息提醒
                        this.modifybookVisible = false // 将对话框设置为不可见
                        this.Querybooks() // 重新查询借书证以刷新页面
                    }
                    else {
                        ElMessage.success("图书修改成功~") // 显示消息提醒
                        this.modifybookVisible = false // 将对话框设置为不可见
                        this.Querybooks() // 重新查询借书证以刷新页面
                    }
                })
        },
        ConfirmRemovebook() {
            axios.delete('/book', { params: { toRemove: this.toRemove } })
                .then(response =>{
                    if (response.data === "Book removed successfully"){
                        ElMessage.success("图书删除成功")
                        this.removebookVisible = false
                        this.Querybooks()
                    } else {
                        ElMessage.error("图书删除失败，该书有人正在借阅~")
                        this.removebookVisible = false
                        this.Querybooks()
                    }   
                })
        },
        ConfirmSearchbook() {
            this.books = []
            let response = axios.get('/book', {params: {toSearch: JSON.stringify(this.toSearch)}}) // 向/book发出GET请求
                .then(response => {
                    if (response.data === "number constrain"){
                        ElMessage.error("查询请求失败，请确认出版年份/价格是数字")
                        this.Querybooks()
                    } else {
                        ElMessage.success("查询请求成功！")
                        this.searchbookVisible = false
                        let books = response.data // 接收响应负载
                        books.forEach(book => { // 对于每个图书
                        this.books.push(book) // 将其加入到列表中
                        })
                    }
                })

        },
        Querybooks() {
            this.books = [] // 清空列表
            let response = axios.get('/book') // 向/book发出GET请求
                .then(response => {
                        let books = response.data // 接收响应负载
                        books.forEach(book => { // 对于每个图书
                        this.books.push(book) // 将其加入到列表中
                    })
                })
        }
    },
    mounted() { // 当页面被渲染时
        this.Querybooks() // 查询借书证
    }
}

</script>


<style scoped>
.bookBox {
    height: 530px;
    width: 200px;
    box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
    text-align: center;
    margin-top: 40px;
    margin-left: 27.5px;
    margin-right: 10px;
    padding: 7.5px;
    padding-right: 10px;
    padding-top: 15px;
}

.newbookBox {
    height: 530px;
    width: 200px;
    margin-top: 40px;
    margin-left: 27.5px;
    margin-right: 10px;
    padding: 7.5px;
    padding-right: 10px;
    padding-top: 15px;
    box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
    text-align: center;
}
</style>