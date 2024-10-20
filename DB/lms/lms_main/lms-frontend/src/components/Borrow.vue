<template>
    <el-scrollbar height="100%" style="width: 100%;">

        <!-- 标题和搜索框 -->
        <div style="margin-top: 20px; margin-left: 40px; font-size: 2em; font-weight: bold;">
            借书记录查询/还书
            <el-input v-model="toSearch" :prefix-icon="Search"
                style=" width: 15vw;min-width: 150px; margin-left: 30px; margin-right: 30px; float: right; ;"
                clearable />
        </div>

        <!-- 查询框 -->
        <div style="width:30%;margin:0 auto; padding-top:5vh;">

            <el-input v-model="this.toQuery" style="display:inline; " placeholder="输入借书证ID"></el-input>
            <el-button style="margin-left: 10px;" type="primary" @click="QueryBorrows">查询</el-button>

        </div>

        <!-- 结果表格 -->
        <el-table v-if="isShow" :data="fitlerTableData" height="600"
            :default-sort="{ prop: 'borrowTime', order: 'ascending' }" :table-layout="'auto'"
            style="width: 100%; margin-left: 50px; margin-top: 30px; margin-right: 50px; max-width: 80vw;">
            <el-table-column prop="cardID" label="借书证ID" />
            <el-table-column prop="bookID" label="图书ID" sortable />
            <el-table-column prop="borrowTime" label="借出时间" sortable />
            <el-table-column prop="returnTime" label="归还时间" sortable />
            <el-table-column fixed="right" label="操作" width="120">
            <template #default="{row}">
                <el-button link type="primary" size="small" @click="returnbookVisible = true
                , returnbookinfo.bookID = row.bookID, returnbookinfo.cardID = row.cardID,
                returnbookinfo.borrowTime = row.borrowTime, returnbookinfo.returnTime = row.returnTime">归还书籍</el-button>
            </template>
            </el-table-column>
        </el-table>

        <!-- 归还图书对话框 -->  
        <el-dialog v-model="returnbookVisible" :title="'归还图书'" width="30%">
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                您要归还的图书信息如下(已归还的书籍不被允许归还)：
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                图书ID: {{ this.returnbookinfo.bookID }}
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                借书证ID: {{ this.returnbookinfo.cardID }}
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                借书时间: {{ this.returnbookinfo.borrowTime }}
            </div>
            <div style="margin-left: 2vw; font-weight: bold; font-size: 1rem; margin-top: 20px; ">
                还书时间: {{ this.returnbookinfo.returnTime }}
            </div>
            <template #footer>
                <span class="dialog-footer">
                    <el-button @click="returnbookVisible = false">取消</el-button>
                    <el-button type="primary" @click="returnbook"
                        :disabled="returnbookinfo.returnTime != '未归还'">确定</el-button>
                </span>
            </template>
        </el-dialog>

    </el-scrollbar>
</template>

<script>
import axios from 'axios';
import { Search } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus';

export default {
    data() {
        return {
            isShow: false, // 结果表格展示状态
            tableData: [{ // 列表项
                cardID: 1,
                bookID: 1,
                borrowTime: "2024.03.04 21:48",
                returnTime: "2024.03.04 21:49"
            }],
            toQuery: '', // 待查询内容(对某一借书证号进行查询)
            toSearch: '', // 待搜索内容(对查询到的结果进行搜索)
            returnbookVisible: false,
            returnbookinfo: {
                bookID: '',
                cardID: '',
                borrowTime: '',
                returnTime: ''
            },

            Search
        }
    },
    computed: {
        fitlerTableData() { // 搜索规则
            return this.tableData.filter(
                (tuple) =>
                    (this.toSearch == '') || // 搜索框为空，即不搜索
                    tuple.bookID == this.toSearch || // 图书号与搜索要求一致
                    tuple.borrowTime.toString().includes(this.toSearch) || // 借出时间包含搜索要求
                    tuple.returnTime.toString().includes(this.toSearch) // 归还时间包含搜索要求
            )
        }
    },
    methods: {
        returnbook() {
            axios.put("/borrow",
                {
                    bookID: this.returnbookinfo.bookID,
                    cardID: this.returnbookinfo.cardID
                })
                .then(response =>{
                    if (response.data === "book returned failed"){
                        ElMessage.error("归还图书失败！")
                        this.returnbookVisible = false
                    } else if (response.data === "book returned successfully"){
                        ElMessage.success("归还图书成功！")
                        this.returnbookVisible = false
                        this.QueryBorrows()
                    }
                })
        },
        async QueryBorrows() {
            this.tableData = [] // 清空列表
            let response = await axios.get('/borrow', { params: { cardID: this.toQuery } }) // 向/borrow发出GET请求，参数为cardID=this.toQuery
            let borrows = response.data // 获取响应负载
            borrows.forEach(borrow => { // 对于每一个借书记录
                this.tableData.push(borrow) // 将它加入到列表项中
            });
            this.isShow = true // 显示结果列表
        }
    }
}
</script>