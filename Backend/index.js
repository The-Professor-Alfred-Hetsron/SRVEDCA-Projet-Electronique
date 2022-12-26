const mongoose = require('mongoose')
const User = require('./models/User')

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
        // const user = await User.where('age').gte(18).lte(30).where("name", "Aurel").limit(2).populate("bestFriend")
        const user = await User.find().byName('Talla A')
        // user.name = 'Carlos Kameni'
        // user.email = "carloskameni@yahoo.fr"
        // user.age = 36
        // await user.save()
        // user[0].bestFriend = '63a8a253be49c13afba9570b'
        // await user[0].save()
        //console.log(user)
        await user[0].save()
        console.log(user)
        //user.forEach((a_user)=> a_user.sayHi())
        user.forEach((a_user)=> console.log(a_user.namedEmail))
    
    } catch (err) {
        console.error(err.message)
    }
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
}