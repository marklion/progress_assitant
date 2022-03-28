<template>
<div class="container">
    <div class=nav-bar>
        <div class="logo-container">
            <img src="https://wpimg.wallstcn.com/69a1c46c-eb1c-4b46-8bd4-e9e686ef5251.png" class="sidebar-logo">
            <h1 class="sidebar-title">掌易助理管理平台</h1>
        </div>
        <div class="menu-container">
            <ul>
                <li class="active"><van-icon name="user-o">客户管理</van-icon></li>
            </ul>
        </div>
    </div>
</div>
</template>

<script>


export default {
    data() {
        return {

        }
    }
}
</script>

<style scoped>
.container {
    height: 100px;
}

.nav-bar {
    height: 50px;
    background-color: #304156;
    box-shadow: 0 4px 8px #7e7979;
}

.nav-bar .logo-container {
    display: inline-block;
    width: 210px;
    padding: 0 15px
}

.nav-bar .menu-container {
    display: inline-block;
}

.nav-bar .menu-container li{
    line-height: 50px;
    padding: 0 12px;
}

.nav-bar .menu-container .active {
    color : #fff;
    background: #2b2f3a;
}

.sidebar-logo {
    width: 32px;
    height: 32px;
    vertical-align: middle;
    margin-right: 12px;
}

.sidebar-title {
    display: inline-block;
    margin: 0;
    color: #fff;
    font-weight: 600;
    line-height: 50px;
    font-size: 14px;
    font-family: Avenir, Helvetica Neue, Arial, Helvetica, sans-serif;
    vertical-align: middle;
}
</style>
