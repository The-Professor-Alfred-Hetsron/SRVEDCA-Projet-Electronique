// Importation des biblioteques
const express = require('express')
const mongoose = require('mongoose')
const morgan = require('morgan')
const bodyParser = require('body-parser')
const dotenv = require('dotenv')
const cors = require('cors');


// Configuration des variables d'environement1
dotenv.config({
    path : "./API/config/.env"
})

// Importation des routes
const AdminRoute = require('./API/Routes/admin')
const AuthRoute = require('./API/Routes/auth')
const ClasseRoute = require('./API/Routes/classe')
const CoursRoute = require('./API/Routes/cours')
const EtudiantRoute = require('./API/Routes/etudiant')
const PlanningRoute = require('./API/Routes/planning')
const PresenceRoute = require('./API/Routes/presence')


// Connexion a la BD
const DBURL = process.env.DBURL || 'mongodb+srv://user-aurel:amCgrWF32p0rTXts@cluster0.swo0hmy.mongodb.net/projet_electronique?retryWrites=true&w=majority'

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
app.use(cors());

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