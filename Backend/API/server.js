// Importation des biblioteques
const express = require('express')
const mongoose = require('mongoose')
const morgan = require('morgan')
const bodyParser = require('body-parser')
const dotenv = require('dotenv')


// Configuration des variables d'environement1
dotenv.config({
    path : "./config/.env"
})

// Importation des routes
const AdminRoute = require('./Routes/admin')
const AuthRoute = require('./Routes/auth')
const ClasseRoute = require('./Routes/classe')
const CoursRoute = require('./Routes/cours')
const EtudiantRoute = require('./Routes/etudiant')
const PlanningRoute = require('./Routes/planning')
const PresenceRoute = require('./Routes/presence')


// Connexion a la BD
const DBURL = process.env.DBURL || 'mongodb://localhost:27017/srvedcaDB'

mongoose.set('strictQuery', true)

mongoose.connect(DBURL, {useNewUrlParser: true, useUnifiedTopology: true})
const db = mongoose.connection

db.once('open', ()=>{
    console.log('Database Connection Established!')
})

db.on('error', (err)=>{
    console.log(err)
})


// Creation du serveur express
const app = express()

app.use(morgan('dev'))
app.use(bodyParser.urlencoded({extended: true}))
app.use(bodyParser.json())

const PORT = process.env.PORT || 8080

app.listen(PORT, () =>{
    console.log(`Server is running on port ${PORT}...`)
})


// Utilisation des routes dans le serveur
app.use('/api/admin', AdminRoute)
app.use('/api/auth', AuthRoute)
app.use('/api/classe', ClasseRoute)
app.use('/api/cours', CoursRoute)
app.use('/api/etudiant', EtudiantRoute)
app.use('/api/planning', PlanningRoute)
app.use('/api/presence', PresenceRoute)