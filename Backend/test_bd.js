const mongoose = require('mongoose')
const Classe = require('./Database/Classe')
const Cours = require('./Database/Cours')
const Etudiant = require('./Database/Etudiant')
const Planning = require('./Database/Planning')
const Presence = require('./Database/Presence')
const moment = require('moment')
// const User = require('./models/User')

mongoose.set('strictQuery', true)

mongoose.connect("mongodb://localhost/projet_electronique")
        .then(()=>{
            console.log("Connected to the database")
        })
        .catch((err)=>{
            console.error("Error while connecting:", err.message)
        })

run()

async function run(){
    try {

        //Testons la récupération des plannings des cours
        // const planning = await Planning.find().populate('cours').sort({jourSemaine: 1, heureDebut: 1})
        
        // for(const plan of planning){
        //     let a_plan = {jourSemaine: plan.jourSemaineText, heureDebut: plan.hDebut, heureFin: plan.hFin, cours: plan.cours.nom}
        //     console.log(a_plan)
        // }

        //Testons la récupération des présences
        const presences = await Presence.find().sort({dateheure: 1}).populate('etudiant').exec()
        let i = 1
        for(const presence of presences){
            console.log('##########', i, '##########')
            let a_presence = {
                //etudiant: await presence.etudiant.populate('classe'),
                etudiant: presence.etudiant,
                cours: await presence.getCours(),
                date: presence.date,
                heure: presence.heure,
                dateheure: presence.dateheure
            }
            console.log(a_presence)
            i++
        }


        // const user = await User.where('age').gte(18).lte(30).where("name", "Aurel").limit(2).populate("bestFriend")
        // const user = await User.find().byName('Talla A')
        // user.name = 'Carlos Kameni'
        // user.email = "carloskameni@yahoo.fr"
        // user.age = 36
        // await user.save()
        // user[0].bestFriend = '63a8a253be49c13afba9570b'
        // await user[0].save()
        //console.log(user)
        // await user[0].save()
        // console.log(user)
        //user.forEach((a_user)=> a_user.sayHi())
        // user.forEach((a_user)=> console.log(a_user.namedEmail))
    
    } catch (err) {
        console.error(err.message)
    }
}

function getCourseId(code, cours){
    const a_cours = cours.find((value) => value.code === code)
    return a_cours._id
}

function getEtudiantId(name, etudiants){
    const a_student = etudiants.find((value) => value.nom === name)
    return a_student._id
}

async function Oldrun(){
    try {
        const user = new User({
            name: "Marie Pierre",
            age: 25,
            email: 'tallatalla@gmail.com',
            hobbies: ["Natation", "Mangas"],
            address: {
                street: "Rue Joss", city: 'Douala'
            }
        })
    
       await user.save()

       user.createdAt = 5
       await user.save()
       console.log(user)
    
    } catch (err) {
        console.error(err.message)
    }


    try {
        const classes = await Classe.find().where({nom: '4GI'})
        classe1 = classes[0]
        console.log(classe1)

        const reseau = new Cours({
            code: 'GIF401',
            nom: 'Administration réseau',
            classe: classe1._id,
            semestre: 1
        })
        await reseau.save()
        const anglais = new Cours({
            code: 'GIF402',
            nom: 'Anglais',
            classe: classe1._id,
            semestre: 1
        })
        await anglais.save()
        const cdp = new Cours({
            code: 'GIF403',
            nom: 'Conduite de projets',
            classe: classe1._id,
            semestre: 1
        })
        await cdp.save()
        const datamining = new Cours({
            code: 'GIF404',
            nom: 'Data Mining',
            classe: classe1._id,
            semestre: 1
        })
        await datamining.save()
        const electronique = new Cours({
            code: 'GIF405',
            nom: 'Electronique et interfaçage',
            classe: classe1._id,
            semestre: 1
        })
        await electronique.save()
        const grammaire = new Cours({
            code: 'GIF406',
            nom: 'Grammaire et langages',
            classe: classe1._id,
            semestre: 1
        })
        await grammaire.save()
        const ihm = new Cours({
            code: 'GIF407',
            nom: 'Interaction Homme-Machine',
            classe: classe1._id,
            semestre: 1
        })
        await ihm.save()
        const ml = new Cours({
            code: 'GIF408',
            nom: 'Machine Learning',
            classe: classe1._id,
            semestre: 1
        })
        await ml.save()
        const management = new Cours({
            code: 'GIF409',
            nom: 'Management',
            classe: classe1._id,
            semestre: 1
        })
        await management.save()
        const progweb = new Cours({
            code: 'GIF400',
            nom: 'Programmation web',
            classe: classe1._id,
            semestre: 1
        })
        await progweb.save()

        const etudiants = await Etudiant.create([
            {
                matricule: '19P001', nom: 'ANZIE', classe: classes[0]._id, empreinte: 'abcd', statut: true
            },
            {
                matricule: '19P002', nom: 'ALFRED', classe: classes[0]._id, empreinte: 'hbcd', statut: true
            },
            {
                matricule: '19P003', nom: 'DJOUMESSI', classe: classes[0]._id, empreinte: 'fbcd', statut: true
            },
            {
                matricule: '19P004', nom: 'KEGNE', classe: classes[0]._id, empreinte: 'mbcd', statut: true
            },
            {
                matricule: '19P005', nom: 'MENRA', classe: classes[0]._id, empreinte: 'gbcd', statut: true
            },
            {
                matricule: '19P017', nom: 'ZIINAM', classe: classes[0]._id, empreinte: 'ebcd', statut: true
            }
        ])

        const presences = await Presence.create([
            {
                etudiant: getEtudiantId('ANZIE', etudiants), dateheure: moment({ hour:8, minute:2 }).toDate(),
            },
            {
                etudiant: getEtudiantId('DJOUMESSI', etudiants), dateheure: moment({ hour:8, minute:2 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ZIINAM', etudiants), dateheure: moment({ hour:8, minute:5 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ALFRED', etudiants), dateheure: moment({ hour:8, minute:27 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ALFRED', etudiants), dateheure: moment({ hour:10, minute:15 }).toDate(),
            },
            {
                etudiant: getEtudiantId('DJOUMESSI', etudiants), dateheure: moment({ hour:10, minute:25 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ZIINAM', etudiants), dateheure: moment({ hour:11, minute:18 }).toDate(),
            },
            {
                etudiant: getEtudiantId('MENRA', etudiants), dateheure: moment({ hour:11, minute:30 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ZIINAM', etudiants), dateheure: moment({ hour:13, minute:30 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ALFRED', etudiants), dateheure: moment({ hour:13, minute:31 }).toDate(),
            },
            {
                etudiant: getEtudiantId('DJOUMESSI', etudiants), dateheure: moment({ hour:13, minute:35 }).toDate(),
            },
            {
                etudiant: getEtudiantId('KEGNE', etudiants), dateheure: moment({ hour:13, minute:36 }).toDate(),
            },
            {
                etudiant: getEtudiantId('MENRA', etudiants), dateheure: moment({ hour:15, minute:47 }).toDate(),
            },
            {
                etudiant: getEtudiantId('ALFRED', etudiants), dateheure: moment({ hour:15, minute:59 }).toDate(),
            },
            {
                etudiant: getEtudiantId('DJOUMESSI', etudiants), dateheure: moment({ hour:15, minute:55 }).toDate(),
            },
            {
                etudiant: getEtudiantId('KEGNE', etudiants), dateheure: moment({ hour:16, minute:36 }).toDate(),
            },
        ])

        // const user = await User.where('age').gte(18).lte(30).where("name", "Aurel").limit(2).populate("bestFriend")
        // const user = await User.find().byName('Talla A')
        // user.name = 'Carlos Kameni'
        // user.email = "carloskameni@yahoo.fr"
        // user.age = 36
        // await user.save()
        // user[0].bestFriend = '63a8a253be49c13afba9570b'
        // await user[0].save()
        //console.log(user)
        // await user[0].save()
        // console.log(user)
        //user.forEach((a_user)=> a_user.sayHi())
        // user.forEach((a_user)=> console.log(a_user.namedEmail))
    
    } catch (err) {
        console.error(err.message)
    }
}